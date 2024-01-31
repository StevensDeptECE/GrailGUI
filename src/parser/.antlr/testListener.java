// Generated from /home/dkruger/git/proj/GrailGUI/src/parser/test.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link testParser}.
 */
public interface testListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link testParser#prog}.
	 * @param ctx the parse tree
	 */
	void enterProg(testParser.ProgContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#prog}.
	 * @param ctx the parse tree
	 */
	void exitProg(testParser.ProgContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(testParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(testParser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#simple_statement}.
	 * @param ctx the parse tree
	 */
	void enterSimple_statement(testParser.Simple_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#simple_statement}.
	 * @param ctx the parse tree
	 */
	void exitSimple_statement(testParser.Simple_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#doc}.
	 * @param ctx the parse tree
	 */
	void enterDoc(testParser.DocContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#doc}.
	 * @param ctx the parse tree
	 */
	void exitDoc(testParser.DocContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#graph}.
	 * @param ctx the parse tree
	 */
	void enterGraph(testParser.GraphContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#graph}.
	 * @param ctx the parse tree
	 */
	void exitGraph(testParser.GraphContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#inc}.
	 * @param ctx the parse tree
	 */
	void enterInc(testParser.IncContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#inc}.
	 * @param ctx the parse tree
	 */
	void exitInc(testParser.IncContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#compound_statement}.
	 * @param ctx the parse tree
	 */
	void enterCompound_statement(testParser.Compound_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#compound_statement}.
	 * @param ctx the parse tree
	 */
	void exitCompound_statement(testParser.Compound_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#quiz}.
	 * @param ctx the parse tree
	 */
	void enterQuiz(testParser.QuizContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#quiz}.
	 * @param ctx the parse tree
	 */
	void exitQuiz(testParser.QuizContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#table}.
	 * @param ctx the parse tree
	 */
	void enterTable(testParser.TableContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#table}.
	 * @param ctx the parse tree
	 */
	void exitTable(testParser.TableContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#paramlist}.
	 * @param ctx the parse tree
	 */
	void enterParamlist(testParser.ParamlistContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#paramlist}.
	 * @param ctx the parse tree
	 */
	void exitParamlist(testParser.ParamlistContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#param}.
	 * @param ctx the parse tree
	 */
	void enterParam(testParser.ParamContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#param}.
	 * @param ctx the parse tree
	 */
	void exitParam(testParser.ParamContext ctx);
	/**
	 * Enter a parse tree produced by {@link testParser#path}.
	 * @param ctx the parse tree
	 */
	void enterPath(testParser.PathContext ctx);
	/**
	 * Exit a parse tree produced by {@link testParser#path}.
	 * @param ctx the parse tree
	 */
	void exitPath(testParser.PathContext ctx);
}