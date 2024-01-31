// Generated from simple_grail.g4 by ANTLR 4.13.1

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ErrorNode;
import org.antlr.v4.runtime.tree.TerminalNode;

import java.util.HashMap;
/**
 * This class provides an empty implementation of {@link simple_grailListener},
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
@SuppressWarnings("CheckReturnValue")
public class simple_grailTagsImpl extends simple_grailBaseListener {
    private static HashMap<String, Integer> opCodeToTag;
    private static String[] tagNames;
    static {
        tagNames = new String[]{
            "p", "div", "rect", "button", "name", "txt",
        };
        opCodeToTag = new HashMap<>();
        int opcode = 0;
        for (String t : tagNames)
            opCodeToTag.put(t, opcode++);        
    }
    private int[] opcodes;
    private int[] intData;
    private float[] floatData;
    private String[] stringData;
    int numInstructions;
    int numInts;
    int numFloats;
    int numStrings;
    public simple_grailTagsImpl(){
        final int maxInstructions = 1 << 16;
        opcodes     = new int[maxInstructions];
        intData     = new int[maxInstructions];
        floatData   = new float[maxInstructions];
        stringData  = new String[maxInstructions];
    }
	@Override public void enterProg(simple_grailParser.ProgContext ctx) {
        numInstructions = 0;
        numInts = 0;
        numFloats = 0;
        numStrings = 0;
        System.out.println("Starting program size=0");
    }

	@Override public void enterTag_block(simple_grailParser.Tag_blockContext ctx) { }
	@Override public void exitTag_block(simple_grailParser.Tag_blockContext ctx) { }
	@Override public void enterTag(simple_grailParser.TagContext ctx) { }
	@Override public void exitTag(simple_grailParser.TagContext ctx) { }
	@Override public void enterP(simple_grailParser.PContext ctx) { 
        System.out.println("enter P");
    }
	@Override public void exitP(simple_grailParser.PContext ctx) { 
        System.out.println("exit P");
    }
	@Override public void enterDiv(simple_grailParser.DivContext ctx) { }
	@Override public void exitDiv(simple_grailParser.DivContext ctx) { }
	@Override public void enterCanvas(simple_grailParser.CanvasContext ctx) { }
	@Override public void exitCanvas(simple_grailParser.CanvasContext ctx) { }
	@Override public void enterRect(simple_grailParser.RectContext ctx) { }
	@Override public void exitRect(simple_grailParser.RectContext ctx) { }
	@Override public void enterButton(simple_grailParser.ButtonContext ctx) { }
	@Override public void exitButton(simple_grailParser.ButtonContext ctx) { }
	@Override public void enterTxt(simple_grailParser.TxtContext ctx) { }
	@Override public void exitTxt(simple_grailParser.TxtContext ctx) { }
	@Override public void enterName(simple_grailParser.NameContext ctx) { }
	@Override public void exitName(simple_grailParser.NameContext ctx) { }
    @Override public void enterTextlit(simple_grailParser.TextlitContext ctx) { }
	@Override public void exitTextlit(simple_grailParser.TextlitContext ctx) {
        System.out.println(ctx.toString());

    }
}