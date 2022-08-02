using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mat;

namespace CsGameEngine.WpfItem.MaterialItems
{
    interface IShaderValue
    {
        void InitData(string name ,ShaderValue shaderValue);
    }
}
