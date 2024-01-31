// Generated from simple_grail.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link simple_grailParser}.
 */
public interface simple_grailListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#prog}.
	 * @param ctx the parse tree
	 */
	void enterProg(simple_grailParser.ProgContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#prog}.
	 * @param ctx the parse tree
	 */
	void exitProg(simple_grailParser.ProgContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#tag_block}.
	 * @param ctx the parse tree
	 */
	void enterTag_block(simple_grailParser.Tag_blockContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#tag_block}.
	 * @param ctx the parse tree
	 */
	void exitTag_block(simple_grailParser.Tag_blockContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#tag}.
	 * @param ctx the parse tree
	 */
	void enterTag(simple_grailParser.TagContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#tag}.
	 * @param ctx the parse tree
	 */
	void exitTag(simple_grailParser.TagContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#p}.
	 * @param ctx the parse tree
	 */
	void enterP(simple_grailParser.PContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#p}.
	 * @param ctx the parse tree
	 */
	void exitP(simple_grailParser.PContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#div}.
	 * @param ctx the parse tree
	 */
	void enterDiv(simple_grailParser.DivContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#div}.
	 * @param ctx the parse tree
	 */
	void exitDiv(simple_grailParser.DivContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#canvas}.
	 * @param ctx the parse tree
	 */
	void enterCanvas(simple_grailParser.CanvasContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#canvas}.
	 * @param ctx the parse tree
	 */
	void exitCanvas(simple_grailParser.CanvasContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#rect}.
	 * @param ctx the parse tree
	 */
	void enterRect(simple_grailParser.RectContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#rect}.
	 * @param ctx the parse tree
	 */
	void exitRect(simple_grailParser.RectContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#button}.
	 * @param ctx the parse tree
	 */
	void enterButton(simple_grailParser.ButtonContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#button}.
	 * @param ctx the parse tree
	 */
	void exitButton(simple_grailParser.ButtonContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#txt}.
	 * @param ctx the parse tree
	 */
	void enterTxt(simple_grailParser.TxtContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#txt}.
	 * @param ctx the parse tree
	 */
	void exitTxt(simple_grailParser.TxtContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#textlit}.
	 * @param ctx the parse tree
	 */
	void enterTextlit(simple_grailParser.TextlitContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#textlit}.
	 * @param ctx the parse tree
	 */
	void exitTextlit(simple_grailParser.TextlitContext ctx);
	/**
	 * Enter a parse tree produced by {@link simple_grailParser#name}.
	 * @param ctx the parse tree
	 */
	void enterName(simple_grailParser.NameContext ctx);
	/**
	 * Exit a parse tree produced by {@link simple_grailParser#name}.
	 * @param ctx the parse tree
	 */
	void exitName(simple_grailParser.NameContext ctx);
}