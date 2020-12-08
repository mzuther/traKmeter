mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: default clean

default : Resources/Skin.xml \
          Resources/background/stereo.svg \
          Resources/background/multi.svg \
          Source/skin.h


Resources/Skin.xml : Original/Skin.xml
	@echo
	@echo "Copying \"Skin.xml\"..."
	@mkdir -p "${mkfile_dir}/Resources"
	@cp "${mkfile_dir}/Original/Skin.xml" "${mkfile_dir}/Resources/Skin.xml"
	@echo "Done."
	@echo


Resources/background/stereo.svg : Original/Skin.svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py "Original/Skin.svg" "Resources"


Resources/background/multi.svg : Original/Skin.svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py "Original/Skin (Multi).svg" "Resources"


Source/skin.h : Resources/background/stereo.svg \
                Resources/background/multi.svg \
                Resources/Skin.xml
	@mkdir -p "${mkfile_dir}/Source"
	@cd ${mkfile_dir} ; ../Source/frut/convert_resources.py "Resources" "Source" "trakmeter::skin"


clean :
	@rm -rf "${mkfile_dir}/Resources"
	@rm -rf "${mkfile_dir}/Source"
