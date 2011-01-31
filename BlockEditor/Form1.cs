using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using BoxEngine.ChunkEngine;

namespace BlockEditor
{
	public partial class editor_Form : Form
	{
		private BlockDictionary mDictionary;
		private String mFilename;

		public editor_Form()
		{
			InitializeComponent();

			mDictionary = new BlockDictionary();
			mFilename = null;

			String filter = "Block Dictionaries (*.dict)|*.dict|All Files|*";
			dialog_Open.Filter = filter;
			dialog_Save.Filter = filter;
		}

		private void dict_New(object sender, EventArgs e)
		{
			blockDetails.Columns.Clear();
			blockDetails.Rows.Clear();

			AddDefaultColumns();

			mDictionary = new BlockDictionary();
			mFilename = null;
		}

		private void dict_Open(object sender, EventArgs e)
		{
			if (dialog_Open.ShowDialog() == DialogResult.OK)
			{
				String filename = dialog_Open.FileName;

				try
				{
					blockDetails.Columns.Clear();
					blockDetails.Rows.Clear();

					mDictionary = BlockDictionary.FromFile(filename);

					List<String> columns = mDictionary.GetAttributes();
					foreach ( String col in columns )
					{
						blockDetails.Columns.Add(col, col);
					}

					Dictionary<int, BlockTemplate>.Enumerator templateEnum = mDictionary.GetEnumerator();
					while ( templateEnum.MoveNext() )
					{
						int rowID = blockDetails.Rows.Add();

						Dictionary<String, object>.Enumerator attrEnum = templateEnum.Current.Value.GetEnumerator();
						while ( attrEnum.MoveNext() )
						{
							blockDetails.Rows[rowID].Cells[attrEnum.Current.Key].Value = attrEnum.Current.Value;
						}
					}
				}
				catch (System.Exception ex)
				{
					System.Windows.Forms.MessageBox.Show("Error loading dictionary:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}	
			}
		}

		private void dict_Save(object sender, EventArgs e)
		{
			String filename = GetSaveFilename(false);

			if (!String.IsNullOrEmpty(filename))
			{
				SyncDictionary();
				mDictionary.ToFile(filename);
			}
		}

		private void dict_SaveAs(object sender, EventArgs e)
		{
			String filename = GetSaveFilename(true);

			if (!String.IsNullOrEmpty(filename))
			{
				SyncDictionary();
				mDictionary.ToFile(filename);
			}
		}

		private String GetSaveFilename(bool forceDialog)
		{
			if ( forceDialog || String.IsNullOrEmpty(mFilename) )
			{
				if ( dialog_Save.ShowDialog() == DialogResult.OK )
				{
					mFilename = dialog_Save.FileName;
				}
			}

			return mFilename;
		}

		private void SyncDictionary()
		{
			// Sync the table with the dict
			mDictionary.ClearAttributes();
			mDictionary.ClearTemplates();

			foreach (DataGridViewColumn col in blockDetails.Columns)
			{
				mDictionary.AddAttribute(col.Name);
			}

			foreach ( DataGridViewRow row in blockDetails.Rows )
			{
				if (!row.IsNewRow)
				{
					BlockTemplate t = new BlockTemplate();

					foreach ( DataGridViewCell cell in row.Cells )
					{
						String name = cell.OwningColumn.Name;
						char attrType = name[0];
						object attrValue = null;

						switch (attrType)
						{
							case 's':
								try
								{
									attrValue = Convert.ToString(cell.Value);
								}
								catch (Exception ex)
								{
									MessageBox.Show(String.Format("Error converting cell {0}:{1} to string.", cell.RowIndex, cell.ColumnIndex));
									attrValue = String.Empty;
								}
								break;
							case 'i':
								try
								{
									attrValue = Convert.ToInt32(cell.Value);
								}
								catch (Exception ex)
								{
									MessageBox.Show(String.Format("Error converting cell {0}:{1} to int.", cell.RowIndex, cell.ColumnIndex));
									attrValue = 0;
								}
								break;
							case 'f':
								try
								{
									attrValue = Convert.ToSingle(cell.Value);
								}
								catch (Exception ex)
								{
									MessageBox.Show(String.Format("Error converting cell {0}:{1} to float.", cell.RowIndex, cell.ColumnIndex));
									attrValue = 0.0f;
								}
								break;
							case 'b':
								try
								{
									attrValue = Convert.ToBoolean(cell.Value);
								}
								catch (Exception ex)
								{
									MessageBox.Show(String.Format("Error converting cell {0}:{1} to boolean.", cell.RowIndex, cell.ColumnIndex));
									attrValue = false;
								}
								break;
						}

						t.SetAttribute(name, attrValue);
					}

					mDictionary.SetTemplate(t.GetAttribute<int>("iID"), t);
				}
			}
		}

		private void AddDefaultColumns()
		{
			blockDetails.Columns.Add("iID", "iID");
			blockDetails.Columns.Add("sName", "sName");

			blockDetails.Columns.Add("fTexture", "fTexture");
			blockDetails.Columns.Add("bOcclude", "bOcclude");

			blockDetails.Columns.Add("iValue", "iValue");
			blockDetails.Columns.Add("iHealth", "iHealth");
			blockDetails.Columns.Add("fStrength", "fStrength");
			blockDetails.Columns.Add("iSticky", "iSticky");
			blockDetails.Columns.Add("fMovement", "fMovement");

			blockDetails.Columns.Add("fGravityX", "fGravityX");
			blockDetails.Columns.Add("fGravityY", "fGravityY");
			blockDetails.Columns.Add("fGravityZ", "fGravityZ");
		}
	}
}
