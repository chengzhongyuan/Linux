test:test.cc
	g++ -o $@ $^ -O3 -std=c++11
.PHONY:clean
clean:
	rm -f test

#  --param name=value -O  -O0  -O1  -O2  -O3  -Os -Ofast -Og
# 这些参数就是可以加入进去，让编译器自行进行优化
#        Preprocessor Options
#            -Aquestion=answer -A-question[=answer] -C  -dD  -dI  -dM  -dN -Dmacro[=defn]  -E  -H
#            -idirafter dir -include file  -imacros file -iprefix file  -iwithprefix dir
#            -iwithprefixbefore dir  -isystem dir -imultilib dir -isysroot dir -M  -MM  -MF  -MG
#            -MP  -MQ  -MT  -nostdinc -P  -fdebug-cpp -ftrack-macro-expansion -fworking-directory
#            -remap -trigraphs  -undef  -Umacro -Wp,option -Xpreprocessor option -no-integrated-cpp