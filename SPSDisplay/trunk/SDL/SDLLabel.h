//
//  SDLLabel.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__SDLLabel__
#define __SPSDisplay__SDLLabel__

#include "SDLView.h"


//
//	Standard Includes
//

#include <string>


class SDLFont;
class SDLSurface;

enum
SDLJustification
{
	kJustificationLeft			=	0,
	kJustificationCenter		=	1,
	kJustificationRight			=	2,
};

class
SDLLabel : public SDLView
{
public:
						/**
							The frame is used for justification; the label will
							be as large as the text and font dictate.
						*/
				
						SDLLabel(const SDL_Rect& inFrame);
						SDLLabel(int16_t inX, int16_t inY, uint16_t inWidth, uint16_t inHeight);
				
	void				setText(const std::string& inVal);
	void				setFont(const SDLFont* inVal);
	void				setTextColor(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inAlpha = 255);
	void				setJustification(SDLJustification inJust);
	
	virtual	void		draw(SDLSurface* inSurface) const;

	const SDLSurface*	surface() const;
	
protected:
	void				init();
	void				updateSurface();
	
private:
	std::string			mText;
	const SDLFont*		mFont;
	SDL_Color			mTextColor;
	SDLJustification	mJustification;
	SDLSurface*			mSurface;
};










#endif /* defined(__SPSDisplay__SDLLabel__) */
