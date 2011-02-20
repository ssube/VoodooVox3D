#include "WorldLoader.hpp"

WorldLoader::WorldLoader(string name)
{
	int rc = sqlite3_open(name.c_str(), &mDatabase);
	if ( rc )
	{
		fprintf(stderr, "Unable to open SQLite database (file \"%s\").\n", name.c_str());
		sqlite3_close(mDatabase);
		return;
	}

	sqlite3_exec(mDatabase, "CREATE TABLE IF NOT EXISTS 'chunks';", NULL, NULL, NULL);

	rc = sqlite3_prepare_v2(mDatabase, "SELECT blocks FROM chunks WHERE ( x = ?1 AND y = ?2 AND z = ?3 );", -1, &mStmtLoad, NULL);
	rc = sqlite3_prepare_v2(mDatabase, "INSERT OR REPLACE INTO chunks ( px, py, pz, blocks ) VALUES ( ?1, ?2, ?3, ?4 );", -1, &mStmtSave, NULL);
}

WorldLoader::~WorldLoader()
{
	sqlite3_finalize(mStmtLoad);
	sqlite3_finalize(mStmtSave);

	if ( mDatabase )
	{
		sqlite3_close(mDatabase);
	}
}

bool WorldLoader::LoadChunk(ivec3 position, size_t * blocks, void * data)
{
	int rc = sqlite3_bind_int(mStmtLoad, 1, position.x);
	rc = sqlite3_bind_int(mStmtLoad, 1, position.y);
	rc = sqlite3_bind_int(mStmtLoad, 1, position.z);

	rc = sqlite3_step(mStmtLoad);

	if ( rc == SQLITE_ROW )
	{
		const void * cdata = sqlite3_column_blob(mStmtLoad, 0);

		if ( cdata )
		{
			int size = sqlite3_column_bytes(mStmtLoad, 0);
			*blocks = size / 2;

			memcpy(data, cdata, size);
		} else {
			data = NULL;
			*blocks = 0;
		}

		sqlite3_reset(mStmtLoad);
	}
}

bool WorldLoader::SaveChunk(ivec3 position, size_t * blocks, void * data)
{

}

void * WorldLoader::Compress(size_t * size, void * data)
{
	unsigned char * startbuffer = (unsigned char*)data;
	unsigned char * endbuffer = (unsigned char*)malloc(size);
	size_t endsize = 0;

	for ( size_t pos = 0; pos < size; ++pos )
	{
		if ( pos == size )
		{
			endbuffer[endsize++] = startbuffer[pos];
		} else {
			if ( startbuffer[pos] != startbuffer[pos+1] )
			{
				endbuffer[endsize++] = startbuffer[pos];
			} else {
				// May be the start of a run, find the end
				unsigned char runlen = 0;
				unsigned char runchar = startbuffer[pos];
				while ( startbuffer[pos] == runchar && runlen < 255 )
				{
					++runlen;
					++pos;
				}

				if ( runlen > 3 )
				{
					// Write the run
					endbuffer[endsize++] = 0xFF;
					endbuffer[endsize++] = runlen;
					endbuffer[endsize++] = runchar;
				} else {
					for ( unsigned char i = 0; i < runlen; ++i )
					{
						endbuffer[endsize++] = runchar;
					}
				}
			}
		}
	}

	endbuffer = realloc(endbuffer, endsize);
	*blocks = endsize;
	return endbuffer;
}

void * WorldLoader::Decompress(size_t size, void * data)
{

}
