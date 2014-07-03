//
//  SDLView.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__SDLView__
#define __SPSDisplay__SDLView__

//
//	Standard Includes
//

#include <vector>

//
//	Library Includes
//

#include <SDL/SDL.h>


class SDLSurface;



class
SDLView
{
public:
							SDLView(const SDL_Rect& inFrame);
							SDLView(int16_t inX, int16_t inY, uint16_t inWidth, uint16_t inHeight);
	
	const SDL_Rect&			frame()							const	{ return mFrame; }
	void					setBackgroundColor(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inAlpha = 255);
	const SDL_Color&		backgroundColor()				const	{ return mBackgroundColor; }
	
	void					setNeedsDisplay()						{ mNeedsDisplay = true; }
	bool					needsDisplay()					const	{ return mNeedsDisplay; }
	
	virtual	void			draw(SDLSurface* inSurface) const;

	typedef	std::vector<SDLView*>		ViewVectorT;
	
	ViewVectorT&			subviews()								{ return mSubviews; }
	void					addSubview(SDLView* inView);
	
protected:
	void					clearNeedsDisplay()						{ mNeedsDisplay = false; }
	
private:
	SDL_Rect				mFrame;
	bool					mNeedsDisplay;
	ViewVectorT				mSubviews;
	SDL_Color				mBackgroundColor;
};


#endif /* defined(__SPSDisplay__SDLView__) */
