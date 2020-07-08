#include "Subsection.h"

/*
	Implementation file for Class Subsection
*/

/*  Constructor  */
Subsection::Subsection()
{

}


/*  Modifiers  */
/*  Accessors  */
/*  Functions  */

void Subsection::addRow(const DataRow &r) {
	rows.push_back(r);
}
