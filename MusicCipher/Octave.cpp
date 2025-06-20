#include "Octave.hpp"


Octave::Octave(unsigned number, unsigned transposition, unsigned symbolsIndex)
{
	assert(symbolsIndex < 3);
	m_symbolsIndex = symbolsIndex;

	for (unsigned i = 0; i < length; ++i)
	{
		m_numbers.push_back( number + (i + transposition) / length );
		m_semitones.append( StandardOctave[(i + transposition) % length] );
	}
}

QString Octave::getSemitone(unsigned idx) const
{
	assert(idx < length);
	return m_semitones[int(idx)];
}

unsigned Octave::getOctaveNumber(unsigned idx) const
{
	assert(idx < length);
	return m_numbers[idx];
}

char Octave::getSymbol(unsigned idx) const
{
	assert(idx < length);
	return SymbolArrays[m_symbolsIndex][idx];
}

unsigned Octave::findSemitone(QString semitone) const
{
	assert( m_semitones.contains(semitone) );
	return unsigned(m_semitones.indexOf(semitone));
}

QString Octave::normalize(const QString& semitone)
{
	if(FlatteredToSharpened.find(semitone) != FlatteredToSharpened.end())
		return FlatteredToSharpened.at(semitone);
	else
		return semitone;
}
