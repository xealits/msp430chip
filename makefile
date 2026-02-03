%.html: %.md
	#pandoc --from markdown-markdown_in_html_blocks+raw_html $< > $@
	pandoc --from markdown-markdown_in_html_blocks+raw_html --standalone --template template.html $< > $@
