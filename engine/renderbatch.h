//Account for the map and the maximum number of entities to show

/** Render flags */
#define BATCHFLAG_UNIFORM_TEXTURE		0x1

/** Used for storing data for batch renders */
typedef struct RenderBatch_T
{
	struct RenderBatch_T *next;		/**< Pointer to the next renderbatch */
	Vec2f vertices[MAX_VERTICES];   /**< List of vertices that may be repeated */
	Vec2f UVs[MAX_VERTICES];	    /**< List of UVs that may not be repeated */
	GLuint indices[MAX_INDICES];	/**< List of indices that will not be repeated */
	GLfloat alpha_mod[MAX_INDICES];	/**< Alpha mods for each index */
	GLuint textures[MAX_VERTICES/4];/**< Texture for the render batch */
	GLuint shader_id;				/**< The shader ID to be used for this render batch */
	short num_vertices;				/**< Number of vertices in the batch */
	short num_indices;				/**< Number of indices in the batch */
	short num_textures;				/**< Number of texture ids */
	long flags;						/**< Renderbatch flags */
}RenderBatch_T;

void BatchEntity(Entity_T *s, Rect *new_dest);
void BatchScreenMap(RenderBatch_T *batch, Map_T *t, Vec2f center);
void BatchScreenEntities(RenderBatch_T *batch, Map_T *map, Vec2f center);
void DrawBatch(RenderBatch_T *batch, Shader_T *s);
void ResetBatch(RenderBatch_T *b);