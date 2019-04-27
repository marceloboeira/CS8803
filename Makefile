.PHONY: docs
docs: docs_generate_table_of_contents docs_join

.PHONY: generate_table_of_contents
docs_generate_table_of_contents:
	@ls -al ./lessons/| grep -o 'lesson-\d\+' | gsort -V | sed 's/.*/* [&](lessons\/&#&)/;s/\[l/\[L/;s/\-/ /' > docs/README-TOC.md

.PHONY: docs_join
docs_join:
	@cat docs/README-HEAD.md docs/README-TOC.md docs/README-FOOTER.md > README.md
