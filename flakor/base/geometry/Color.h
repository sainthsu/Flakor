
#ifndef _FK_COLOR_H_
#define _FK_COLOR_H_

#include <stdint.h>

class Color
{
	public:
		static const float COLOR_FACTOR_INT_TO_FLOAT = 255.0f;
		static const int32_t ABGR_PACKED_RED_SHIFT = 0;
		static const int32_t ABGR_PACKED_GREEN_SHIFT = 8;
		static const int32_t ABGR_PACKED_BLUE_SHIFT = 16;
		static const int32_t ABGR_PACKED_ALPHA_SHIFT = 24;

		static const int32_t ABGR_PACKED_RED_CLEAR = 0XFFFFFF00;
		static const int32_t ABGR_PACKED_GREEN_CLEAR = 0XFFFF00FF;
		static const int32_t ABGR_PACKED_BLUE_CLEAR = 0XFF00FFFF;
		static const int32_t ABGR_PACKED_ALPHA_CLEAR = 0X00FFFFFF;
	
		static const int32_t RGBA_PACKED_ALPHA_SHIFT = 0;
		static const int32_t RGBA_PACKED_BLUE_SHIFT = 8;
		static const int32_t RGBA_PACKED_GREEN_SHIFT = 16;
		static const int32_t RGBA_PACKED_RED_SHIFT = 24;
	
		static const int32_t RGBA_PACKED_ALPHA_CLEAR = 0XFFFFFF00;
		static const int32_t RGBA_PACKED_BLUE_CLEAR = 0XFFFF00FF;
		static const int32_t RGBA_PACKED_GREEN_CLEAR = 0XFF00FFFF;
		static const int32_t RGBA_PACKED_RED_CLEAR = 0X00FFFFFF;
	
		static const int32_t ARGB_PACKED_BLUE_SHIFT = 0;
		static const int32_t ARGB_PACKED_GREEN_SHIFT = 8;
		static const int32_t ARGB_PACKED_RED_SHIFT = 16;
		static const int32_t ARGB_PACKED_ALPHA_SHIFT = 24;

		static const int32_t ARGB_PACKED_BLUE_CLEAR = 0XFFFFFF00;
		static const int32_t ARGB_PACKED_GREEN_CLEAR = 0XFFFF00FF;
		static const int32_t ARGB_PACKED_RED_CLEAR = 0XFF00FFFF;
		static const int32_t ARGB_PACKED_ALPHA_CLEAR = 0X00FFFFFF;
	
		static const Color WHITE = Color(1, 1, 1, 1);
		static const Color BLACK = Color(0, 0, 0, 1);
		static const Color RED = Color(1, 0, 0, 1);
		static const Color YELLOW = Color(1, 1, 0, 1);
		static const Color GREEN = Color(0, 1, 0, 1);
		static const Color CYAN = Color(0, 1, 1, 1);
		static const Color BLUE = Color(0, 0, 1, 1);
		static const Color PINK = Color(1, 0, 1, 1);
		static const Color TRANSPARENT = Color(0, 0, 0, 0);
		
		float red;
		float green;
		float blue;
		float alpha;

	public:
		Color();
		Color(float red,float green,float blue);
		Color(float red,float green,float blue,float alpha);
		Color(const Color& other);

		void setColor(float red,float green,float blue);
		void setColor(float red,float green,float blue,float alpha);
		void setColor(const Color& other);

    	bool equals(const Color& target) const;

		inline void reset()
		{
			setColor(*Color::WHITE);
		}

		inline bool setRedChecking(float red)
		{
			if(this.red != red)
			{
				this.red = red;
				return true;
			}
			return false;
		}

		inline bool setGreenChecking(float green)
		{
			if(this.green != green)
			{
				this.green = green;
				return true;
			}
			return false;
		}

		inline bool setBlueChecking(float blue)
		{
			if(this.blue != blue)
			{
				this.blue = blue;
				return true;
			}
			return false;
		}

		inline bool setAlphaChecking(float alpha)
		{
			if(this.alpha != alpha)
			{
				this.alpha = alpha;
				return true;
			}
			return false;
		}
		
}

#endif
