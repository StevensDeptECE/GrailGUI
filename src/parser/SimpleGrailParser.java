import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
public class SimpleGrailParser {
    public static void main(String[] args) {
//        String filename = args.length > 0 ? args[0] : "simple_grail.g4";
String text = "<p>text is here</p>";
        simple_grailLexer lexer = new simple_grailLexer(CharStreams.fromString(text));
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        simple_grailParser parser = new simple_grailParser(tokens);
        simple_grailTagsImpl listener= new simple_grailTagsImpl();
        ParseTree tree = parser.prog();
        ParseTreeWalker.DEFAULT.walk(listener, tree);
    }
}
