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
			protected Dictionary<String, object> mAttributes;

			public BlockTemplate()
			{
				mAttributes = new Dictionary<string, object>();
			}

			public BlockTemplate(BlockTemplate template)
			{
				mAttributes = template.mAttributes;
			}

			public T GetAttribute<T>(String key)
			{
				object value = null;
				mAttributes.TryGetValue(key, out value);
				try
				{
					T real = (T)value;
					return real;
				}
				catch (System.Exception ex)
				{
					Console.WriteLine("Unable to cast block attribute {0} from {1} to {2}, error:\n\t{3}", key, typeof(T).Name, value.GetType().Name, ex.Message);
					object error = null;					
					return (T)error;
				}
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
        public class Block : BlockTemplate
        {
			public Block(BlockTemplate template) : base(template)
			{
			}
        }
    }
}