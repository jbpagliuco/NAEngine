#pragma once

#include "RenderableInstance.h"

namespace na
{
	class VertexBuffer;
	class IndexBuffer;
	class Material;

	class MeshInstance : public RenderableInstance
	{
	public:
		bool Initialize(VertexBuffer *vertexBuffer, IndexBuffer *indexBuffer, Material *material);
		void Shutdown();

		virtual void Render() override;

	private:
		VertexBuffer *mVertexBuffer;
		IndexBuffer *mIndexBuffer;

		Material *mMaterial;
	};
}