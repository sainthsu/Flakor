#include "core/opengl/vbo/VBO.h"

FLAKOR_NS_BEGIN

/**
 * VBO =  VertexBufferObject
 */

VBO::VBO()
{
}

VBO::~VBO()
{
}

VBO VBO::create(int sizePerVertex,int vertexNumber)
{
}

/**
* 是否使用后自动销毁
* @return true if auto
*/
bool VBO::isAutoDispose();

		/**
		 * 获取该VBO对应的显卡bufferID
		 * @return int bufferID
		 */
		int VBO::getBufferID();
		/**
		 * 是否已载入显卡	
		 * @return true if loaded
		 */
		bool VBO::isLoaded();
		/**
		 * 标记VBO对象没有载入显卡硬件。如果要再次使用此VBO，将重载
		 * Mark this {@link VertexBufferObject} as not not loaded to hardware.
		 * It will reload itself to hardware when it gets used again.
		 */
		void VBO::setNotLoaded();
		/**
		 * 从显卡上卸载下来
		 * @param glState Opengle 状态
		 */
		void VBO::unload(final GLState glState);

		/**
		 * 是否VBO数据已过期
		 * @return true if its dirty
		 */
		bool VBO::isDirty();

		/**
		 * 标记数据已过期
		 * Mark this {@link VertexBufferObject} dirty so it gets updated on the hardware.
		 */
		void VBO::setDirty();


		int VBO::getSizePerVertex() const;
		int VBO::getVertexNumber() const;

		/**
		 * 浮点数的容量数
		 * @return the number of <code>float</code>s that fit into this {@link VBOInterface}.
		 */
		int VBO::getCapacity();

		/**
		 * Byte数的容量数
		 * @return the number of <code>byte</code>s that fit into this {@link VBOInterface}.
		 */
		int VBO::getByteCapacity();

		/**
		 * 获取显卡CPU中的Byte大小
		 * @return the number of <code>byte</code>s that are allocated on the GPU.
		 */
		int VBO::getGPUMemoryByteSize();

		/**
		 * 与显卡bufferID绑定
		 * @param glState
		 */
		void VBO::bind(GLState glStatae);
		void VBO::bind(GLState glState, ShaderProgram shaderProgram);
		void VBO::unbind(GLState glState, ShaderProgram shaderProgram);

		VBOManager VBO::getVBOManager();

		/**
		 * 画VBO存储的信息
		 * @param primitiveType int
		 * @param count int
		 */
		void VBO::draw(final int primitiveType, final int count);
		void VBO::draw(final int primitiveType, final int offset, final int count);

		virtual void onBufferData();


FLAKOR_NS_END

#endif
