using System;
using System.Collections.Generic;
using SlimDX;
using System.Runtime.Serialization;

namespace BoxEngine
{
    namespace ChunkEngine
    {
        [Serializable]
        public class BlockTemplate
        {
			private Dictionary<String, object> mAttributes;

			public BlockTemplate()
			{
				mAttributes = new Dictionary<string, object>();
			}

			public object GetAttribute(String key)
			{
				object value = null;
				mAttributes.TryGetValue(key, out value);
				return value;
			}

			public void SetAttribute(String key, object value)
			{
				if (mAttributes.ContainsKey(key))
				{
					mAttributes[key] = value;
				} else {
					mAttributes.Add(key, value);
				}
			}

			public Dictionary<String, object>.Enumerator GetEnumerator()
			{
				return mAttributes.GetEnumerator();
			}
		}

        [Serializable]
        public class Block
        {
            private BlockTemplate mTemplate;

            public Block(BlockTemplate template) 
            {
                mTemplate = template;
            }
        }
    }
}