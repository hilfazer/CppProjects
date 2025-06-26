#include "Octave.hpp"

const char SymbolArrays[3][13] = { {"ABCDEFGHIJKL"}, {"MNOPQRSTUVWX"}, {"YZ0123456789"} };


Octave::Octave(unsigned startingNumber, unsigned transposition, unsigned symbolsIndex)
{
	assert(symbolsIndex < 3);
	m_symbolsIndex = symbolsIndex;

	for (unsigned i = 0; i < octaveLength; ++i)
	{
		m_numbers.push_back( startingNumber + (i + transposition) / octaveLength );
		m_semitones.append( StandardOctave[(i + transposition) % octaveLength] );
	}
}

QString Octave::getSemitone(unsigned idx) const
{
	assert(idx < octaveLength);
	return m_semitones[int(idx)];
}

unsigned Octave::getOctaveNumber(unsigned idx) const
{
	assert(idx < octaveLength);
	return m_numbers[idx];
}

char Octave::getSymbol(unsigned idx) const
{
	assert(idx < octaveLength);
	return SymbolArrays[m_symbolsIndex][idx];
}

unsigned Octave::findSemitone(QString semitone) const
{
	assert( m_semitones.contains(semitone) );
	return unsigned(m_semitones.indexOf(semitone));
}

QString Octave::normalize(const QString& semitone)
{
	if ( !AllSemitones.contains(semitone) )
		throw NotASemitoneException{semitone};

	if(FlattenedToSharpened.find(semitone) != FlattenedToSharpened.end())
		return FlattenedToSharpened.at(semitone);
	else
		return semitone;
}


NotASemitoneException::NotASemitoneException(const QString &str)
	: m_str(str.toStdString())
{
}

const char *NotASemitoneException::what() const
{
	return m_str.c_str();
}


