%.html: %.md
	#pandoc --from markdown-markdown_in_html_blocks+raw_html $< > $@
	pandoc --standalone --template template.html $< > $@
