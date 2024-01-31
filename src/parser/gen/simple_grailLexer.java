// Generated from simple_grail.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class simple_grailLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, CLASS_NAME=17, 
		ASSIGN_COORD=18, NAME=19, ASSIGN_TEXT=20, TEXT=21;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
			"T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "CLASS_NAME", 
			"ASSIGN_COORD", "NAME", "ASSIGN_TEXT", "TEXT"
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


	public simple_grailLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "simple_grail.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\u0004\u0000\u0015\u008d\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002"+
		"\u0001\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002"+
		"\u0004\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002"+
		"\u0007\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002"+
		"\u000b\u0007\u000b\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e"+
		"\u0002\u000f\u0007\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011"+
		"\u0002\u0012\u0007\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014"+
		"\u0001\u0000\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002"+
		"\u0001\u0003\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005"+
		"\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006"+
		"\u0001\u0007\u0001\u0007\u0001\b\u0001\b\u0001\t\u0001\t\u0001\n\u0001"+
		"\n\u0001\u000b\u0001\u000b\u0001\u000b\u0001\f\u0001\f\u0001\f\u0001\f"+
		"\u0001\f\u0001\f\u0001\f\u0001\r\u0001\r\u0001\r\u0001\r\u0001\r\u0001"+
		"\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000f\u0001"+
		"\u000f\u0001\u0010\u0001\u0010\u0005\u0010d\b\u0010\n\u0010\f\u0010g\t"+
		"\u0010\u0001\u0011\u0001\u0011\u0004\u0011k\b\u0011\u000b\u0011\f\u0011"+
		"l\u0001\u0012\u0001\u0012\u0005\u0012q\b\u0012\n\u0012\f\u0012t\t\u0012"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0005\u0013y\b\u0013\n\u0013\f\u0013"+
		"|\t\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0005\u0013\u0081\b\u0013"+
		"\n\u0013\f\u0013\u0084\t\u0013\u0001\u0013\u0003\u0013\u0087\b\u0013\u0001"+
		"\u0014\u0004\u0014\u008a\b\u0014\u000b\u0014\f\u0014\u008b\u0000\u0000"+
		"\u0015\u0001\u0001\u0003\u0002\u0005\u0003\u0007\u0004\t\u0005\u000b\u0006"+
		"\r\u0007\u000f\b\u0011\t\u0013\n\u0015\u000b\u0017\f\u0019\r\u001b\u000e"+
		"\u001d\u000f\u001f\u0010!\u0011#\u0012%\u0013\'\u0014)\u0015\u0001\u0000"+
		"\u0006\u0003\u0000AZ__az\u0004\u000009AZ__az\u0001\u000009\u0002\u0000"+
		"\"\"^^\u0002\u0000\'\'^^\u0001\u0000<<\u0093\u0000\u0001\u0001\u0000\u0000"+
		"\u0000\u0000\u0003\u0001\u0000\u0000\u0000\u0000\u0005\u0001\u0000\u0000"+
		"\u0000\u0000\u0007\u0001\u0000\u0000\u0000\u0000\t\u0001\u0000\u0000\u0000"+
		"\u0000\u000b\u0001\u0000\u0000\u0000\u0000\r\u0001\u0000\u0000\u0000\u0000"+
		"\u000f\u0001\u0000\u0000\u0000\u0000\u0011\u0001\u0000\u0000\u0000\u0000"+
		"\u0013\u0001\u0000\u0000\u0000\u0000\u0015\u0001\u0000\u0000\u0000\u0000"+
		"\u0017\u0001\u0000\u0000\u0000\u0000\u0019\u0001\u0000\u0000\u0000\u0000"+
		"\u001b\u0001\u0000\u0000\u0000\u0000\u001d\u0001\u0000\u0000\u0000\u0000"+
		"\u001f\u0001\u0000\u0000\u0000\u0000!\u0001\u0000\u0000\u0000\u0000#\u0001"+
		"\u0000\u0000\u0000\u0000%\u0001\u0000\u0000\u0000\u0000\'\u0001\u0000"+
		"\u0000\u0000\u0000)\u0001\u0000\u0000\u0000\u0001+\u0001\u0000\u0000\u0000"+
		"\u0003-\u0001\u0000\u0000\u0000\u0005/\u0001\u0000\u0000\u0000\u00071"+
		"\u0001\u0000\u0000\u0000\t3\u0001\u0000\u0000\u0000\u000b7\u0001\u0000"+
		"\u0000\u0000\r>\u0001\u0000\u0000\u0000\u000fC\u0001\u0000\u0000\u0000"+
		"\u0011E\u0001\u0000\u0000\u0000\u0013G\u0001\u0000\u0000\u0000\u0015I"+
		"\u0001\u0000\u0000\u0000\u0017K\u0001\u0000\u0000\u0000\u0019N\u0001\u0000"+
		"\u0000\u0000\u001bU\u0001\u0000\u0000\u0000\u001dZ\u0001\u0000\u0000\u0000"+
		"\u001f_\u0001\u0000\u0000\u0000!a\u0001\u0000\u0000\u0000#h\u0001\u0000"+
		"\u0000\u0000%n\u0001\u0000\u0000\u0000\'u\u0001\u0000\u0000\u0000)\u0089"+
		"\u0001\u0000\u0000\u0000+,\u0005<\u0000\u0000,\u0002\u0001\u0000\u0000"+
		"\u0000-.\u0005p\u0000\u0000.\u0004\u0001\u0000\u0000\u0000/0\u0005>\u0000"+
		"\u00000\u0006\u0001\u0000\u0000\u000012\u0005/\u0000\u00002\b\u0001\u0000"+
		"\u0000\u000034\u0005d\u0000\u000045\u0005i\u0000\u000056\u0005v\u0000"+
		"\u00006\n\u0001\u0000\u0000\u000078\u0005c\u0000\u000089\u0005a\u0000"+
		"\u00009:\u0005n\u0000\u0000:;\u0005v\u0000\u0000;<\u0005a\u0000\u0000"+
		"<=\u0005s\u0000\u0000=\f\u0001\u0000\u0000\u0000>?\u0005r\u0000\u0000"+
		"?@\u0005e\u0000\u0000@A\u0005c\u0000\u0000AB\u0005t\u0000\u0000B\u000e"+
		"\u0001\u0000\u0000\u0000CD\u0005x\u0000\u0000D\u0010\u0001\u0000\u0000"+
		"\u0000EF\u0005y\u0000\u0000F\u0012\u0001\u0000\u0000\u0000GH\u0005w\u0000"+
		"\u0000H\u0014\u0001\u0000\u0000\u0000IJ\u0005h\u0000\u0000J\u0016\u0001"+
		"\u0000\u0000\u0000KL\u0005/\u0000\u0000LM\u0005>\u0000\u0000M\u0018\u0001"+
		"\u0000\u0000\u0000NO\u0005b\u0000\u0000OP\u0005u\u0000\u0000PQ\u0005t"+
		"\u0000\u0000QR\u0005t\u0000\u0000RS\u0005o\u0000\u0000ST\u0005n\u0000"+
		"\u0000T\u001a\u0001\u0000\u0000\u0000UV\u0005t\u0000\u0000VW\u0005e\u0000"+
		"\u0000WX\u0005x\u0000\u0000XY\u0005t\u0000\u0000Y\u001c\u0001\u0000\u0000"+
		"\u0000Z[\u0005n\u0000\u0000[\\\u0005a\u0000\u0000\\]\u0005m\u0000\u0000"+
		"]^\u0005e\u0000\u0000^\u001e\u0001\u0000\u0000\u0000_`\u0005=\u0000\u0000"+
		"` \u0001\u0000\u0000\u0000ae\u0007\u0000\u0000\u0000bd\u0007\u0001\u0000"+
		"\u0000cb\u0001\u0000\u0000\u0000dg\u0001\u0000\u0000\u0000ec\u0001\u0000"+
		"\u0000\u0000ef\u0001\u0000\u0000\u0000f\"\u0001\u0000\u0000\u0000ge\u0001"+
		"\u0000\u0000\u0000hj\u0005=\u0000\u0000ik\u0007\u0002\u0000\u0000ji\u0001"+
		"\u0000\u0000\u0000kl\u0001\u0000\u0000\u0000lj\u0001\u0000\u0000\u0000"+
		"lm\u0001\u0000\u0000\u0000m$\u0001\u0000\u0000\u0000nr\u0007\u0000\u0000"+
		"\u0000oq\u0007\u0001\u0000\u0000po\u0001\u0000\u0000\u0000qt\u0001\u0000"+
		"\u0000\u0000rp\u0001\u0000\u0000\u0000rs\u0001\u0000\u0000\u0000s&\u0001"+
		"\u0000\u0000\u0000tr\u0001\u0000\u0000\u0000u\u0086\u0005=\u0000\u0000"+
		"vz\u0005\"\u0000\u0000wy\u0007\u0003\u0000\u0000xw\u0001\u0000\u0000\u0000"+
		"y|\u0001\u0000\u0000\u0000zx\u0001\u0000\u0000\u0000z{\u0001\u0000\u0000"+
		"\u0000{}\u0001\u0000\u0000\u0000|z\u0001\u0000\u0000\u0000}\u0087\u0005"+
		"\"\u0000\u0000~\u0082\u0005\'\u0000\u0000\u007f\u0081\u0007\u0004\u0000"+
		"\u0000\u0080\u007f\u0001\u0000\u0000\u0000\u0081\u0084\u0001\u0000\u0000"+
		"\u0000\u0082\u0080\u0001\u0000\u0000\u0000\u0082\u0083\u0001\u0000\u0000"+
		"\u0000\u0083\u0085\u0001\u0000\u0000\u0000\u0084\u0082\u0001\u0000\u0000"+
		"\u0000\u0085\u0087\u0005\'\u0000\u0000\u0086v\u0001\u0000\u0000\u0000"+
		"\u0086~\u0001\u0000\u0000\u0000\u0087(\u0001\u0000\u0000\u0000\u0088\u008a"+
		"\b\u0005\u0000\u0000\u0089\u0088\u0001\u0000\u0000\u0000\u008a\u008b\u0001"+
		"\u0000\u0000\u0000\u008b\u0089\u0001\u0000\u0000\u0000\u008b\u008c\u0001"+
		"\u0000\u0000\u0000\u008c*\u0001\u0000\u0000\u0000\b\u0000elrz\u0082\u0086"+
		"\u008b\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}