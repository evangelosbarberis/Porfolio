"""
This file defines the database models
"""

import datetime
from .common import db, Field, auth
from pydal.validators import *

def get_user_email():
    return auth.current_user.get('email') if auth.current_user else None

def get_time():
    return datetime.datetime.utcnow()

# shopping list table, stores with user and purchase status, very important
db.define_table('shopping_list',
    Field('user_email', default=get_user_email),
    Field('item_name', requires=IS_NOT_EMPTY()),
    Field('is_purchased', 'boolean', default=False),
    Field('created_at', 'datetime', default=get_time),
)

db.commit()