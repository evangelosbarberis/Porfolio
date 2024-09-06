SOURCES = src

.PHONY: clean

format: $(wildcard $(SOURCES)/*.cpp)
	clang-format --style=Google -i $^

verify-format: $(wildcard $(SOURCES)/*.cpp)
	clang-format --style=Google -i --dry-run --Werror $^

report.pdf: report.md
	pandoc --from=markdown \
       --to=latex \
       --toc \
       -o $@ \
       $^ \
       --shift-heading-level-by -1 \
       --number-sections

clean:
	rm -f report.pdf
