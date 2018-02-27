env=debug

macconf:
	cd ../ && mkdir -p build_$(env)/js && cp -rf js/*.js build_$(env)/js && \
        mkdir -p build_$(env)/grapher.app/Contents/MacOS/js && cp -rf js/*.js build_$(env)/grapher.app/Contents/MacOS/js

macbin:
	cd ../ && rm -rf build_$(env)/*.cpp build_$(env)/*.h build_$(env)/*.o  && \
 	rm -rf package_$(env) && cp -rf build_$(env) package_$(env) && rm -rf package_$(env)/Makefile && \
	~/Qt5.9/5.9.1/clang_64/bin/macdeployqt package_$(env)/grapher.app && \
	ln -s package_$(env)/grapher.app/Contents/MacOS/grapher package_$(env)/grapher && \
	strip -S -x package_$(env)/grapher.app/Contents/MacOS/grapher && \
	rm -rf grapher_$(env).zip && zip -r grapher_$(env).zip package_$(env)/ && rm -rf package_$(env)
