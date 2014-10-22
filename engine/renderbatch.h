//Account for the map and the maximum number of entities to show


/** Used for storing data for batch renders */
typedef struct RenderBatch_T
{
	Vec2f vertices[MAX_VERTICES]; /**< List of vertices that may be repeated */
	Vec2f UVs[MAX_VERTICES];	    /**< List of UVs that may not be repeated */
	GLuint indices[MAX_INDICES];	/**< List of indices that will not be repeated */
	GLfloat alpha_mod[MAX_INDICES];	/**< Alpha mods for each index */
	short tex_ids[MAX_BILLBOARDS];  /**< List of texture IDs for the render batch */
	GLuint shader_id;				/**< The shader ID to be used for this render batch */
	short num_vertices;				/**< Number of vertices in the batch */
	short num_indices;				/**< Number of indices in the batch */
	
}RenderBatch_T;

void BatchEntity(Entity_T *s, Rect *new_dest);
void BatchScreenMap(RenderBatch_T *batch, Map_T *t, Vec2i tile);
