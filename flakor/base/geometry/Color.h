
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
		
		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;

	public:
		Color();
		Color(GLubyte red,GLubyte green,GLubyte blue);
		Color(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha);
		Color(GLfloat red,GLfloat green,GLfloat blue);
		Color(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
		Color(const Color& other);

		void setColor(GLubyte red,GLubyte green,GLubyte blue);
		void setColor(GLubyte red,GLubyte green,GLubyte blue,GLubyte alpha);
		void setColor(GLfloat red,GLfloat green,GLfloat blue);
		void setColor(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
		void setColor(const Color& other);

    	bool equals(const Color& target) const;

		inline void reset()
		{
			setColor(*Color::WHITE);
		}

		inline void setRed(GLubyte red)
		{
			this.red = red/255.f;
		}
		
		inline void setGreen(GLubyte green)
		{
			this.green = green/255.f;
		}

		inline void setBlue(GLubyte blue)
		{
			this.blue = blue/255.f;
		}

		inline void setAlpha(GLubyte alpha)
		{
			this.alpha = alpha/255.f;
		}


		inline bool setRedChecking(GLfloat red)
		{
			if(this.red != red)
			{
				this.red = red;
				return true;
			}
			return false;
		}

		inline bool setGreenChecking(GLfloat green)
		{
			if(this.green != green)
			{
				this.green = green;
				return true;
			}
			return false;
		}

		inline bool setBlueChecking(GLfloat blue)
		{
			if(this.blue != blue)
			{
				this.blue = blue;
				return true;
			}
			return false;
		}

		inline bool setAlphaChecking(GLfloat alpha)
		{
			if(this.alpha != alpha)
			{
				this.alpha = alpha;
				return true;
			}
			return false;
		}
		
		/*public not need to have getter
		inline GLfloat getRed()
		{
			return this.red;
		}
	
		inline GLfloat getGreen()
		{
			return this.green;
		}

		inline GLfloat getBlue()
		{
			return this.blue;
		}

		inline GLfloat getAlpha()
		{
			return this.alpha;
		}
		*/

		inline int32_t getRGBAInt()
		{
				return ((int)(255 * this.alpha) << Color::RGBA_PACKED_ALPHA_SHIFT) | ((int)(255 * red) << Color::RGBA_PACKED_RED_SHIFT) | ((int)(255 * green) << Color::RGBA_PACKED_GREEN_SHIFT) | ((int)(255 * blue) << Color::RGBA_PACKED_BLUE_SHIFT);

		}

		void mix(const Color& ColorA, float percentageA, const Color& ColorB, float percentageB);
		
}

#endif
