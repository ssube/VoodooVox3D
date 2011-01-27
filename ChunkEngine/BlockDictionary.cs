using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters;
using System.Runtime.Serialization.Formatters.Binary;

namespace BoxEngine
{
    namespace ChunkEngine
    {
        [Serializable]
        public class BlockDictionary
        {
			private List<String> mAttributes;
            private Dictionary<int, BlockTemplate> mTemplates;

            static public BlockDictionary FromFile(String filename)
            {
				BlockDictionary dict = null;
				FileStream stream = null;

				try
				{
					stream = File.OpenRead(filename);
					BinaryFormatter format = new BinaryFormatter();
					dict = (BlockDictionary)format.Deserialize(stream);
				}
				catch (System.Exception ex)
				{
					throw ex;
				}
				finally
				{
					if ( stream != null )
					{
						stream.Close();
					}
				}

				return dict;
            }

            public void ToFile(String filename)
            {
				FileStream stream = null;
				try
				{
					stream = File.OpenWrite(filename);
					BinaryFormatter format = new BinaryFormatter();
					format.Serialize(stream, this);
				}
				catch (System.Exception ex)
				{
					throw ex;
				}
				finally
				{
					if (stream != null)
					{
						stream.Close();
					}
				}
            }

            public BlockDictionary()
            {
				mAttributes = new List<string>();
                mTemplates = new Dictionary<int, BlockTemplate>();
            }

			public List<String> GetAttributes()
			{
				return mAttributes;
			}

			public void AddAttribute(String attr)
			{
				mAttributes.Add(attr);
			}

			public void RemoveAttribute(String attr)
			{
				mAttributes.Remove(attr);
			}

			public void ClearAttributes()
			{
				mAttributes.Clear();
			}

            public int GetTemplateCount()
            {
                return mTemplates.Count;
            }

			public List<int> GetTemplateKeys()
			{
				return new List<int>(mTemplates.Keys);
			}

            public BlockTemplate GetTemplate(int n)
            {
                return mTemplates[n];
            }

            public void SetTemplate(int n, BlockTemplate t)
            {
				if (mTemplates.ContainsKey(n))
				{
					mTemplates[n] = t;
				}
				else
				{
					mTemplates.Add(n, t);
				}
            }

            public void ClearTemplates()
            {
                mTemplates.Clear();
            }

			public Dictionary<int, BlockTemplate>.Enumerator GetEnumerator()
			{
				return mTemplates.GetEnumerator();
			}
        }
    }
}
