#include "util/Prefs.hh"
#include <sys/stat.h>
#include <fstream>

using namespace std;
Prefs::Prefs() :
	baseDir(getenv("GRAIL")),
	preferredX(0), preferredY(0),
	preferredWidth(1024), preferredHeight(1024),
	allowMaximize(false),
	logPath(baseDir + "log/log.txt"),
	logLevel(5),
	enableVoiceCmd(false),
	shaderBinaryFormat(0),
	fastLoadShaders(false),
	trySavingShader(true)
{
	baseDir += "/";
}

inline string Prefs::getGrailDir() const {
	string HOME = getenv("HOME"); //TODO: on windows, HOMEPATH
  return HOME + "/.grail/";
}

inline string Prefs::getPath() const {
	return getGrailDir() + "prefs.conf";
}
	
void Prefs::save() {
  ofstream f(getPath().c_str());
	f <<
		baseDir << '\n' <<
		preferredX << '\t' <<	preferredY << '\t' <<	preferredWidth << '\t' <<	preferredHeight << '\n' <<
		allowMaximize << '\n' <<
		logPath << '\n' <<
		logLevel << '\n' <<
		enableVoiceCmd << '\n' <<
		shaderBinaryFormat << '\n' <<
		fastLoadShaders << '\n' <<
		trySavingShader << '\n';
}

void Prefs::load() {
  ifstream f(getPath().c_str());
	if (!f.good()) {
#ifdef __linux__
		mkdir(getGrailDir().c_str(), 0755);
#elif _WIN32
		mkdir(getGrailDir().c_str());
#endif
		save();
		return;
	}
	f >>
		baseDir >>
		preferredX >>	preferredY >>	preferredWidth >>	preferredHeight >>
		allowMaximize >>
		logPath >>
		logLevel >>
		enableVoiceCmd >>
		shaderBinaryFormat >>
		fastLoadShaders >>
		trySavingShader;
}

Prefs prefs;
