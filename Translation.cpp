#include "Translation.h"

#include "Vector.h"

Vector Translation::translation(double time) const
{
	if (time < m_begin)
		return Vector();

	if (time > m_end)
		return m_translation;

	return ((time - m_begin) / (m_end - m_begin)) * m_translation;
}
