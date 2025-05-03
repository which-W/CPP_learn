#include "global.h"


//Ë¢ÐÂqss
std::function<void(QWidget*)> repolish = [](QWidget* w) {
	w->style()->unpolish(w);
	w->style()->polish(w);
};