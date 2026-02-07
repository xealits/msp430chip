%.html: %.md
	#pandoc --from markdown-markdown_in_html_blocks+raw_html $< > $@
	pandoc --from markdown-markdown_in_html_blocks+raw_html --standalone --template template.html $< > $@

device_templates.hpp: prototype_datasheet.html
	python3 generate_headers.py prototype_datasheet.html > device_templates.hpp
