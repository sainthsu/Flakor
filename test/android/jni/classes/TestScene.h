#include "Flakor.h"

using namespace flakor;

class TestScene: public Scene
{
	public:
		virtual bool init() override;
		static TestScene* create();
};
