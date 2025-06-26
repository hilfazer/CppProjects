#ifndef OCTAVE_H
#define OCTAVE_H

#include <QStringList>

#include <map>
#include <vector>

QStringList const StandardOctave = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
QStringList const AllSemitones = StandardOctave + QStringList{"Db", "Eb", "Gb", "Ab", "Bb"};

std::map<QString, QString> const FlattenedToSharpened = { {"Db", "C#"}, {"Eb", "D#"}, {"Gb", "F#"}, {"Ab", "G#"}, {"Bb", "A#"} };

extern const char SymbolArrays[3][13];

class NotASemitoneException : std::exception
{
public:
	NotASemitoneException(QString const& str);
	const char *what() const;

private:
	std::string const m_str;
};

class Octave
{
public:
	static unsigned const octaveLength = 12;

public:
	Octave(unsigned startingNumber, unsigned transposition, unsigned symbolsIndex );

	QString getSemitone(unsigned idx) const;

	unsigned getOctaveNumber(unsigned idx) const;

	char getSymbol(unsigned idx) const;

	unsigned findSemitone(QString standardSemitone) const;

	static QString normalize(QString const& semitone);

private:
	QStringList m_semitones;
	std::vector< unsigned > m_numbers;
	unsigned m_symbolsIndex = 0;
};


#endif // OCTAVE_H
