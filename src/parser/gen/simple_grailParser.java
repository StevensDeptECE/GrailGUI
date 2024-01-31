// Generated from simple_grail.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class simple_grailParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, CLASS_NAME=17, 
		ASSIGN_COORD=18, NAME=19, ASSIGN_TEXT=20, TEXT=21;
	public static final int
		RULE_prog = 0, RULE_tag_block = 1, RULE_tag = 2, RULE_p = 3, RULE_div = 4, 
		RULE_canvas = 5, RULE_rect = 6, RULE_button = 7, RULE_txt = 8, RULE_textlit = 9, 
		RULE_name = 10;
	private static String[] makeRuleNames() {
		return new String[] {
			"prog", "tag_block", "tag", "p", "div", "canvas", "rect", "button", "txt", 
			"textlit", "name"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'<'", "'p'", "'>'", "'/'", "'div'", "'canvas'", "'rect'", "'x'", 
			"'y'", "'w'", "'h'", "'/>'", "'button'", "'text'", "'name'", "'='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, "CLASS_NAME", "ASSIGN_COORD", "NAME", "ASSIGN_TEXT", 
			"TEXT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "simple_grail.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public simple_grailParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ProgContext extends ParserRuleContext {
		public Tag_blockContext tag_block() {
			return getRuleContext(Tag_blockContext.class,0);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterProg(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitProg(this);
		}
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_prog);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(22);
			tag_block();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class Tag_blockContext extends ParserRuleContext {
		public List<TagContext> tag() {
			return getRuleContexts(TagContext.class);
		}
		public TagContext tag(int i) {
			return getRuleContext(TagContext.class,i);
		}
		public Tag_blockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_tag_block; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterTag_block(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitTag_block(this);
		}
	}

	public final Tag_blockContext tag_block() throws RecognitionException {
		Tag_blockContext _localctx = new Tag_blockContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_tag_block);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(27);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,0,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(24);
					tag();
					}
					} 
				}
				setState(29);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,0,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TagContext extends ParserRuleContext {
		public PContext p() {
			return getRuleContext(PContext.class,0);
		}
		public DivContext div() {
			return getRuleContext(DivContext.class,0);
		}
		public CanvasContext canvas() {
			return getRuleContext(CanvasContext.class,0);
		}
		public RectContext rect() {
			return getRuleContext(RectContext.class,0);
		}
		public ButtonContext button() {
			return getRuleContext(ButtonContext.class,0);
		}
		public TxtContext txt() {
			return getRuleContext(TxtContext.class,0);
		}
		public TextlitContext textlit() {
			return getRuleContext(TextlitContext.class,0);
		}
		public TagContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_tag; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterTag(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitTag(this);
		}
	}

	public final TagContext tag() throws RecognitionException {
		TagContext _localctx = new TagContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_tag);
		try {
			setState(37);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,1,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(30);
				p();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(31);
				div();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(32);
				canvas();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(33);
				rect();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(34);
				button();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(35);
				txt();
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(36);
				textlit();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class PContext extends ParserRuleContext {
		public Tag_blockContext tag_block() {
			return getRuleContext(Tag_blockContext.class,0);
		}
		public TerminalNode CLASS_NAME() { return getToken(simple_grailParser.CLASS_NAME, 0); }
		public PContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_p; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterP(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitP(this);
		}
	}

	public final PContext p() throws RecognitionException {
		PContext _localctx = new PContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_p);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(39);
			match(T__0);
			setState(40);
			match(T__1);
			setState(42);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==CLASS_NAME) {
				{
				setState(41);
				match(CLASS_NAME);
				}
			}

			setState(44);
			match(T__2);
			setState(45);
			tag_block();
			setState(46);
			match(T__0);
			setState(47);
			match(T__3);
			setState(48);
			match(T__1);
			setState(49);
			match(T__2);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DivContext extends ParserRuleContext {
		public Tag_blockContext tag_block() {
			return getRuleContext(Tag_blockContext.class,0);
		}
		public TerminalNode CLASS_NAME() { return getToken(simple_grailParser.CLASS_NAME, 0); }
		public DivContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_div; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterDiv(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitDiv(this);
		}
	}

	public final DivContext div() throws RecognitionException {
		DivContext _localctx = new DivContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_div);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(51);
			match(T__0);
			setState(52);
			match(T__4);
			setState(54);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==CLASS_NAME) {
				{
				setState(53);
				match(CLASS_NAME);
				}
			}

			setState(56);
			match(T__2);
			setState(57);
			tag_block();
			setState(58);
			match(T__0);
			setState(59);
			match(T__3);
			setState(60);
			match(T__4);
			setState(61);
			match(T__2);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CanvasContext extends ParserRuleContext {
		public Tag_blockContext tag_block() {
			return getRuleContext(Tag_blockContext.class,0);
		}
		public TerminalNode CLASS_NAME() { return getToken(simple_grailParser.CLASS_NAME, 0); }
		public CanvasContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_canvas; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterCanvas(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitCanvas(this);
		}
	}

	public final CanvasContext canvas() throws RecognitionException {
		CanvasContext _localctx = new CanvasContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_canvas);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(63);
			match(T__0);
			setState(64);
			match(T__5);
			setState(66);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==CLASS_NAME) {
				{
				setState(65);
				match(CLASS_NAME);
				}
			}

			setState(68);
			match(T__2);
			setState(69);
			tag_block();
			setState(70);
			match(T__0);
			setState(71);
			match(T__3);
			setState(72);
			match(T__5);
			setState(73);
			match(T__2);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class RectContext extends ParserRuleContext {
		public List<TerminalNode> ASSIGN_COORD() { return getTokens(simple_grailParser.ASSIGN_COORD); }
		public TerminalNode ASSIGN_COORD(int i) {
			return getToken(simple_grailParser.ASSIGN_COORD, i);
		}
		public TerminalNode CLASS_NAME() { return getToken(simple_grailParser.CLASS_NAME, 0); }
		public RectContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_rect; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterRect(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitRect(this);
		}
	}

	public final RectContext rect() throws RecognitionException {
		RectContext _localctx = new RectContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_rect);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(75);
			match(T__0);
			setState(76);
			match(T__6);
			setState(78);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==CLASS_NAME) {
				{
				setState(77);
				match(CLASS_NAME);
				}
			}

			setState(80);
			match(T__7);
			setState(81);
			match(ASSIGN_COORD);
			setState(82);
			match(T__8);
			setState(83);
			match(ASSIGN_COORD);
			setState(84);
			match(T__9);
			setState(85);
			match(ASSIGN_COORD);
			setState(86);
			match(T__10);
			setState(87);
			match(ASSIGN_COORD);
			setState(88);
			match(T__11);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ButtonContext extends ParserRuleContext {
		public TerminalNode ASSIGN_TEXT() { return getToken(simple_grailParser.ASSIGN_TEXT, 0); }
		public TerminalNode CLASS_NAME() { return getToken(simple_grailParser.CLASS_NAME, 0); }
		public TerminalNode NAME() { return getToken(simple_grailParser.NAME, 0); }
		public ButtonContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_button; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterButton(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitButton(this);
		}
	}

	public final ButtonContext button() throws RecognitionException {
		ButtonContext _localctx = new ButtonContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_button);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(90);
			match(T__0);
			setState(91);
			match(T__12);
			setState(93);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==CLASS_NAME) {
				{
				setState(92);
				match(CLASS_NAME);
				}
			}

			setState(96);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==NAME) {
				{
				setState(95);
				match(NAME);
				}
			}

			setState(98);
			match(T__13);
			setState(99);
			match(ASSIGN_TEXT);
			setState(100);
			match(T__11);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TxtContext extends ParserRuleContext {
		public List<TerminalNode> ASSIGN_COORD() { return getTokens(simple_grailParser.ASSIGN_COORD); }
		public TerminalNode ASSIGN_COORD(int i) {
			return getToken(simple_grailParser.ASSIGN_COORD, i);
		}
		public TxtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_txt; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterTxt(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitTxt(this);
		}
	}

	public final TxtContext txt() throws RecognitionException {
		TxtContext _localctx = new TxtContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_txt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(102);
			match(T__0);
			setState(103);
			match(T__13);
			setState(104);
			match(T__7);
			setState(105);
			match(ASSIGN_COORD);
			setState(106);
			match(T__8);
			setState(107);
			match(ASSIGN_COORD);
			setState(108);
			match(T__11);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class TextlitContext extends ParserRuleContext {
		public TerminalNode TEXT() { return getToken(simple_grailParser.TEXT, 0); }
		public TextlitContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_textlit; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterTextlit(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitTextlit(this);
		}
	}

	public final TextlitContext textlit() throws RecognitionException {
		TextlitContext _localctx = new TextlitContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_textlit);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(110);
			match(TEXT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NameContext extends ParserRuleContext {
		public TerminalNode NAME() { return getToken(simple_grailParser.NAME, 0); }
		public NameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_name; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).enterName(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof simple_grailListener ) ((simple_grailListener)listener).exitName(this);
		}
	}

	public final NameContext name() throws RecognitionException {
		NameContext _localctx = new NameContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_name);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(112);
			match(T__14);
			setState(113);
			match(T__15);
			setState(114);
			match(NAME);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001\u0015u\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0001\u0000\u0001\u0000\u0001"+
		"\u0001\u0005\u0001\u001a\b\u0001\n\u0001\f\u0001\u001d\t\u0001\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0003\u0002&\b\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0003\u0003"+
		"+\b\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0001\u0003\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0004\u0003\u0004"+
		"7\b\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0004\u0001\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0003\u0005"+
		"C\b\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005"+
		"\u0001\u0005\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0006\u0003\u0006"+
		"O\b\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0003\u0007^\b\u0007\u0001\u0007\u0003\u0007"+
		"a\b\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\b\u0001"+
		"\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\t\u0001\t\u0001"+
		"\n\u0001\n\u0001\n\u0001\n\u0001\n\u0000\u0000\u000b\u0000\u0002\u0004"+
		"\u0006\b\n\f\u000e\u0010\u0012\u0014\u0000\u0000v\u0000\u0016\u0001\u0000"+
		"\u0000\u0000\u0002\u001b\u0001\u0000\u0000\u0000\u0004%\u0001\u0000\u0000"+
		"\u0000\u0006\'\u0001\u0000\u0000\u0000\b3\u0001\u0000\u0000\u0000\n?\u0001"+
		"\u0000\u0000\u0000\fK\u0001\u0000\u0000\u0000\u000eZ\u0001\u0000\u0000"+
		"\u0000\u0010f\u0001\u0000\u0000\u0000\u0012n\u0001\u0000\u0000\u0000\u0014"+
		"p\u0001\u0000\u0000\u0000\u0016\u0017\u0003\u0002\u0001\u0000\u0017\u0001"+
		"\u0001\u0000\u0000\u0000\u0018\u001a\u0003\u0004\u0002\u0000\u0019\u0018"+
		"\u0001\u0000\u0000\u0000\u001a\u001d\u0001\u0000\u0000\u0000\u001b\u0019"+
		"\u0001\u0000\u0000\u0000\u001b\u001c\u0001\u0000\u0000\u0000\u001c\u0003"+
		"\u0001\u0000\u0000\u0000\u001d\u001b\u0001\u0000\u0000\u0000\u001e&\u0003"+
		"\u0006\u0003\u0000\u001f&\u0003\b\u0004\u0000 &\u0003\n\u0005\u0000!&"+
		"\u0003\f\u0006\u0000\"&\u0003\u000e\u0007\u0000#&\u0003\u0010\b\u0000"+
		"$&\u0003\u0012\t\u0000%\u001e\u0001\u0000\u0000\u0000%\u001f\u0001\u0000"+
		"\u0000\u0000% \u0001\u0000\u0000\u0000%!\u0001\u0000\u0000\u0000%\"\u0001"+
		"\u0000\u0000\u0000%#\u0001\u0000\u0000\u0000%$\u0001\u0000\u0000\u0000"+
		"&\u0005\u0001\u0000\u0000\u0000\'(\u0005\u0001\u0000\u0000(*\u0005\u0002"+
		"\u0000\u0000)+\u0005\u0011\u0000\u0000*)\u0001\u0000\u0000\u0000*+\u0001"+
		"\u0000\u0000\u0000+,\u0001\u0000\u0000\u0000,-\u0005\u0003\u0000\u0000"+
		"-.\u0003\u0002\u0001\u0000./\u0005\u0001\u0000\u0000/0\u0005\u0004\u0000"+
		"\u000001\u0005\u0002\u0000\u000012\u0005\u0003\u0000\u00002\u0007\u0001"+
		"\u0000\u0000\u000034\u0005\u0001\u0000\u000046\u0005\u0005\u0000\u0000"+
		"57\u0005\u0011\u0000\u000065\u0001\u0000\u0000\u000067\u0001\u0000\u0000"+
		"\u000078\u0001\u0000\u0000\u000089\u0005\u0003\u0000\u00009:\u0003\u0002"+
		"\u0001\u0000:;\u0005\u0001\u0000\u0000;<\u0005\u0004\u0000\u0000<=\u0005"+
		"\u0005\u0000\u0000=>\u0005\u0003\u0000\u0000>\t\u0001\u0000\u0000\u0000"+
		"?@\u0005\u0001\u0000\u0000@B\u0005\u0006\u0000\u0000AC\u0005\u0011\u0000"+
		"\u0000BA\u0001\u0000\u0000\u0000BC\u0001\u0000\u0000\u0000CD\u0001\u0000"+
		"\u0000\u0000DE\u0005\u0003\u0000\u0000EF\u0003\u0002\u0001\u0000FG\u0005"+
		"\u0001\u0000\u0000GH\u0005\u0004\u0000\u0000HI\u0005\u0006\u0000\u0000"+
		"IJ\u0005\u0003\u0000\u0000J\u000b\u0001\u0000\u0000\u0000KL\u0005\u0001"+
		"\u0000\u0000LN\u0005\u0007\u0000\u0000MO\u0005\u0011\u0000\u0000NM\u0001"+
		"\u0000\u0000\u0000NO\u0001\u0000\u0000\u0000OP\u0001\u0000\u0000\u0000"+
		"PQ\u0005\b\u0000\u0000QR\u0005\u0012\u0000\u0000RS\u0005\t\u0000\u0000"+
		"ST\u0005\u0012\u0000\u0000TU\u0005\n\u0000\u0000UV\u0005\u0012\u0000\u0000"+
		"VW\u0005\u000b\u0000\u0000WX\u0005\u0012\u0000\u0000XY\u0005\f\u0000\u0000"+
		"Y\r\u0001\u0000\u0000\u0000Z[\u0005\u0001\u0000\u0000[]\u0005\r\u0000"+
		"\u0000\\^\u0005\u0011\u0000\u0000]\\\u0001\u0000\u0000\u0000]^\u0001\u0000"+
		"\u0000\u0000^`\u0001\u0000\u0000\u0000_a\u0005\u0013\u0000\u0000`_\u0001"+
		"\u0000\u0000\u0000`a\u0001\u0000\u0000\u0000ab\u0001\u0000\u0000\u0000"+
		"bc\u0005\u000e\u0000\u0000cd\u0005\u0014\u0000\u0000de\u0005\f\u0000\u0000"+
		"e\u000f\u0001\u0000\u0000\u0000fg\u0005\u0001\u0000\u0000gh\u0005\u000e"+
		"\u0000\u0000hi\u0005\b\u0000\u0000ij\u0005\u0012\u0000\u0000jk\u0005\t"+
		"\u0000\u0000kl\u0005\u0012\u0000\u0000lm\u0005\f\u0000\u0000m\u0011\u0001"+
		"\u0000\u0000\u0000no\u0005\u0015\u0000\u0000o\u0013\u0001\u0000\u0000"+
		"\u0000pq\u0005\u000f\u0000\u0000qr\u0005\u0010\u0000\u0000rs\u0005\u0013"+
		"\u0000\u0000s\u0015\u0001\u0000\u0000\u0000\b\u001b%*6BN]`";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}