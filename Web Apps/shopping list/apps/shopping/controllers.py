"""
This file defines actions, i.e. functions the URLs are mapped into
The @action(path) decorator exposed the function at URL:

    http://127.0.0.1:8000/{app_name}/{path}

If app_name == '_default' then simply

    http://127.0.0.1:8000/{path}

If path == 'index' it can be omitted:

    http://127.0.0.1:8000/

The path follows the bottlepy syntax.

@action.uses('generic.html')  indicates that the action uses the generic.html template
@action.uses(session)         indicates that the action uses the session
@action.uses(db)              indicates that the action uses the db
@action.uses(T)               indicates that the action uses the i18n & pluralization
@action.uses(auth.user)       indicates that the action requires a logged in user
@action.uses(auth)            indicates that the action requires the auth object

session, db, T, auth, and tempates are examples of Fixtures.
Warning: Fixtures MUST be declared with @action.uses({fixtures}) else your app will result in undefined behavior
"""


from py4web import action, request, abort, redirect, URL
from yatl.helpers import A
from .common import db, session, T, cache, auth, logger, authenticated, unauthenticated, flash
from py4web.utils.url_signer import URLSigner
from .models import get_user_email

url_signer = URLSigner(session)

# provide URLs for client interaction below
@action('index')
@action.uses('index.html', db, auth.user, url_signer)
def index():
    return dict(
        load_data_url = URL('load_data'),
        add_item_url = URL('add_item'),
        update_item_url = URL('update_item'),
        delete_item_url = URL('delete_item'),
        signer = url_signer
    )

# load the user data from the database
@action('load_data')
@action.uses(db, auth.user)
def load_data():
    items = db(db.shopping_list.user_email == get_user_email()).select(
        orderby=~db.shopping_list.created_at  # Sort by creation time, newest first
    )
    return dict(items=[
        {
            'id': item.id,
            'item_name': item.item_name,
            'is_purchased': item.is_purchased
        } for item in items
    ])

# allows us to add a new item to the shopping list
@action('add_item', method="POST")
@action.uses(db, auth.user)
def add_item():
    item_name = request.json.get('item_name')
    id = db.shopping_list.insert(
        item_name=item_name,
        user_email=get_user_email()
    )
    return dict(id=id)


# update the purchase status of the item, update item status
@action('update_item', method="POST")
@action.uses(db, auth.user)
def update_item():
    item_id = request.json.get('id')
    is_purchased = request.json.get('is_purchased')
    if not item_id:
        return dict(status='error')
    db(
        (db.shopping_list.id == item_id) & 
        (db.shopping_list.user_email == get_user_email())
    ).update(is_purchased=is_purchased)
    return dict(status='ok')


# delete the item from the shopping list
@action('delete_item', method="POST")
@action.uses(db, auth.user)
def delete_item():
    item_id = request.json.get('id')
    if not item_id:
        return dict(status='error')
    db(
        (db.shopping_list.id == item_id) & 
        (db.shopping_list.user_email == get_user_email())
    ).delete()
    return dict(status='ok')
