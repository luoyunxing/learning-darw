#include "geometry.h"

Geometry::Geometry()
{

}
Geometry::~Geometry()
{
	if (mVao != 0)
	{
		glDeleteVertexArrays(1, &mVao);
	};

	if (mPosVbo != 0)
	{
		glDeleteBuffers(1, &mPosVbo);
	};

	if (mUvVbo != 0)
	{
		glDeleteBuffers(1, &mUvVbo);
	};

	if (mEbo != 0)
	{
		glDeleteBuffers(1, &mEbo);
	};

	if (mNormalVbo != 0)
	{
		glDeleteBuffers(1, &mNormalVbo);
	};
	
}

Geometry* Geometry::createBox(float size)
{
	Geometry* geometry = new Geometry();

    geometry->mIndicesCount = 36;

    const float halfsize = size / 2.0f;

    // 位置数组 position：xyz
    float Positions[] = {
        // 前面 z = halfsize
        -halfsize, -halfsize,  halfsize,
         halfsize, -halfsize,  halfsize,
         halfsize,  halfsize,  halfsize,
        -halfsize,  halfsize,  halfsize,
        // 后面 z = -halfsize
        -halfsize, -halfsize, -halfsize,
         halfsize, -halfsize, -halfsize,
         halfsize,  halfsize, -halfsize,
        -halfsize,  halfsize, -halfsize,
        // 右面 x = halfsize
         halfsize, -halfsize, -halfsize,
         halfsize, -halfsize,  halfsize,
         halfsize,  halfsize,  halfsize,
         halfsize,  halfsize, -halfsize,
         // 左面 x = -halfsize
         -halfsize, -halfsize, -halfsize,
         -halfsize, -halfsize,  halfsize,
         -halfsize,  halfsize,  halfsize,
         -halfsize,  halfsize, -halfsize,
         // 上面 y = halfsize
         -halfsize,  halfsize, -halfsize,
          halfsize,  halfsize, -halfsize,
          halfsize,  halfsize,  halfsize,
         -halfsize,  halfsize,  halfsize,
         // 下面 y = -halfsize
         -halfsize, -halfsize, -halfsize,
          halfsize, -halfsize, -halfsize,
          halfsize, -halfsize,  halfsize,
         -halfsize, -halfsize,  halfsize,
    };

    // UV数组 uv：u v，和上面顶点一一对应
    float UVs[] = {
        // 前面
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // 后面
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // 右面
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // 左面
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // 上面
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // 下面
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    //法线数据
    float Normals[] = {
        // 前面 z = halfsize，法线 (0,0,1)
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // 后面 z = -halfsize，法线 (0,0,-1)
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // 右面 x = halfsize，法线 (1,0,0)
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // 左面 x = -halfsize，法线 (-1,0,0)
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // 上面 y = halfsize，法线 (0,1,0)
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // 下面 y = -halfsize，法线 (0,-1,0)
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
    };


    // EBO 索引（不变，一共24个顶点，12三角，36索引）
    unsigned int Indices[] = {
        // 前面
        0, 1, 2,
        0, 2, 3,
        // 后面
        4, 5, 6,
        4, 6, 7,
        // 右面
        8, 9,10,
        8,10,11,
        // 左面
        12,13,14,
        12,14,15,
        // 上面
        16,17,18,
        16,18,19,
        // 下面
        20,21,22,
        20,22,23
    };

    //创建vbo,并绑定
    GLuint& posivbo = geometry->mPosVbo;
    GL_CALL(glGenBuffers(1, &posivbo));
    //绑定当前vbo，并写入位置数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posivbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), Positions, GL_STATIC_DRAW));

    //创建uvvbo，并绑定
    GLuint& uvvbo = geometry->mUvVbo;
    glGenBuffers(1, &uvvbo);
    //写入Uv数据
    glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UVs), UVs, GL_STATIC_DRAW);


    //创建normalvbo，并绑定
    GLuint& normalvbo = geometry->mNormalVbo;
    glGenBuffers(1, &normalvbo);
    //写入Uv数据
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals), Normals, GL_STATIC_DRAW);

    //创建vao，并绑定
    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    //创建ebo，并绑定
    GLuint& ebo = geometry->mEbo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    //将位置属性的描述信息加入到vao当中
    glBindBuffer(GL_ARRAY_BUFFER, posivbo);
    //绑定到0号属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

    //将uv属性的描述信息加入到vao当中
    glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));

    //将uv属性的描述信息加入到vao当中
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

    //将ebo绑定到vao当中
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    //解除vao绑定
    glBindVertexArray(0);

	return geometry;
}

Geometry* Geometry::createSphere(float radius)
{
	Geometry* geometry = new Geometry();

    std::vector<GLfloat> Positions{};
    std::vector<GLfloat> UVs{};
    std::vector<GLfloat> Normals{};
    std::vector<GLuint> Indices{};

    //经纬线数量
    int numLatLines = 60;  //纬线
    int numLongLines = 60;  //经线

    for (int i = 0; i <= numLatLines; i++)
    {
        for (int j = 0; j <= numLongLines; j++)
        {
            float phi = i * glm::pi<float>() / numLatLines;
            float theta = j * 2 * glm::pi<float>() / numLongLines;

            float y = radius * cos(phi);
            float x = radius * cos(theta) * sin(phi);
            float z = radius * sin(theta) * sin(phi);

            Positions.push_back(x);
            Positions.push_back(y);
            Positions.push_back(z);

            float u = 1.0 - (float)j / (float)numLongLines;
            float v = 1.0 - (float)i / (float)numLatLines;

            //反转
            //float u = (float)j / (float)numLongLines;
            //float v = (float)i / (float)numLatLines;

            UVs.push_back(u);
            UVs.push_back(v);

            Normals.push_back(x);
            Normals.push_back(y);
            Normals.push_back(z);
           
        }
    }

    //将这些三角形缝补成一个球体
    for (int i = 0; i < numLatLines; i++)
    {
        for (int j = 0; j < numLongLines; j++)
        {
            int p1 = i * (numLongLines + 1) + j;
            int p2 = p1 + numLongLines + 1;
            int p3 = p1 + 1;
            int p4 = p2 + 1;

            Indices.push_back(p1);
            Indices.push_back(p2);
            Indices.push_back(p3);

            Indices.push_back(p3);
            Indices.push_back(p2);
            Indices.push_back(p4);
        }
    }

    //创建vbo,并绑定
    GLuint& posivbo = geometry->mPosVbo;
    GL_CALL(glGenBuffers(1, &posivbo));
    //绑定当前vbo，并写入位置数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posivbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(GLfloat), Positions.data(), GL_STATIC_DRAW));

    //创建uvvbo，并绑定
    GLuint& uvvbo = geometry->mUvVbo;
    glGenBuffers(1, &uvvbo);
    //写入Uv数据
    glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(GLfloat), UVs.data(), GL_STATIC_DRAW);

    //创建normal,并绑定
    GLuint& normalvbo = geometry->mNormalVbo;
    GL_CALL(glGenBuffers(1, &normalvbo));
    //绑定当前vbo，并写入位置数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalvbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(GLfloat), Normals.data(), GL_STATIC_DRAW));

    //创建vao，并绑定
    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    //创建ebo，并绑定
    GLuint& ebo = geometry->mEbo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_STATIC_DRAW);

    //将位置属性的描述信息加入到vao当中
    glBindBuffer(GL_ARRAY_BUFFER, posivbo);
    //绑定到0号属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

    //将uv属性的描述信息加入到vao当中
    glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));

    //将法线属性的描述信息加入到vao当中
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    //绑定到0号属性
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

    //将ebo绑定到vao当中
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    //解除vao绑定
    glBindVertexArray(0);

    geometry->mIndicesCount = Indices.size();

	return geometry;
}

Geometry* Geometry::createFlat(float width, float height)
{
    Geometry* geometry = new Geometry();

    const float halfW = width / 2.0f;
    const float halfH = height / 2.0f;

    // 4 个顶点，位于 XZ 平面，y = 0
    // 顺序：左下、右下、右上、左上（从 +Y 往下看是逆时针）
    float Positions[] = {
       -halfW, -halfH, 0.0f,   // 0 左下
        halfW, -halfH, 0.0f,   // 1 右下
        halfW,  halfH, 0.0f,   // 2 右上
       -halfW,  halfH, 0.0f,   // 3 左上
    };

    // UV 与顶点一一对应
    float UVs[] = {
        0.0f, 1.0f,   // 0
        1.0f, 1.0f,   // 1
        1.0f, 0.0f,   // 2
        0.0f, 0.0f,   // 3
    };

    // 法线全部朝 +Y
    float Normals[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    // 两个三角形，6 个索引
    unsigned int Indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    geometry->mIndicesCount = 6;

    // 位置 VBO
    GLuint& posivbo = geometry->mPosVbo;
    GL_CALL(glGenBuffers(1, &posivbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posivbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), Positions, GL_STATIC_DRAW));

    // UV VBO
    GLuint& uvvbo = geometry->mUvVbo;
    GL_CALL(glGenBuffers(1, &uvvbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvvbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(UVs), UVs, GL_STATIC_DRAW));

    // 法线 VBO
    GLuint& normalvbo = geometry->mNormalVbo;
    GL_CALL(glGenBuffers(1, &normalvbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalvbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Normals), Normals, GL_STATIC_DRAW));

    // VAO
    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    // EBO（在 VAO 绑定期间绑定，记录进 VAO）
    GLuint& ebo = geometry->mEbo;
    GL_CALL(glGenBuffers(1, &ebo));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW));

    // 位置属性 location 0
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posivbo));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

    // UV 属性 location 2
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, uvvbo));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));

    // 法线属性 location 3
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, normalvbo));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

    // EBO 再绑一次（冗余但无害，保证 VAO 记录正确）
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

    glBindVertexArray(0);

    return geometry;
}