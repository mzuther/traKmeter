mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))


default : Default/background/stereo.svg \
          Default/background/multi.svg


Default/background/stereo.svg : Default/Default.svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py "Default/Default.svg" "Default/"


Default/background/multi.svg : Default/Default\ (Multi).svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py "Default/Default (Multi).svg" "Default/"


clean :
	@rm -f Default/background/*.svg
	@rm -f Default/button/*.svg
	@rm -f Default/label/*.svg
	@rm -f Default/meter/*.svg
