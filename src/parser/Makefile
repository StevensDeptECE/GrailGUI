simple1test:
	export CLASSPATH=antlr4-13.0.1-complete.jar:gen:.
	antlr4 -o gen simple_grail.g4
	cd gen
	javac *.java
	cd ..
	javac SimpleGrailParser simple_grailTagsImpl.java
	java org.antlr.v4.gui.TestRig simple_grail prog	 simple1.grail