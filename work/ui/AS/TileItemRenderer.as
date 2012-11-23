import gfx.controls.ListItemRenderer;
import gfx.controls.CoreList;
import gfx.controls.UILoader;
import gfx.controls.Button;

class AS.TileItemRenderer extends ListItemRenderer {

   public var index:Number;
   public var owner:CoreList;
   public var selectable:Boolean = true;
   
   public var thumbImg:UILoader;
   
   public function TileItemRenderer() { super(); }
   
   public function setData(_data:Object):Void 
   {
		this.data = _data;
		//if all data is set, set label
		if (_data._imagePath && _data._label != undefined)
		{
			this.label = _data._label;
		}
		else
		{
			// some data is invalid hide item
			this.visible = false;
		}
	
		invalidate();
   }      
   
   public function draw():Void 
   {
	   //if all data is set, set thumb image
	   	if (data._imagePath != undefined)
		{
			this.visible = true;
			this.thumbImg.visible = true;
			thumbImg.source = this.data._imagePath;
			thumbImg.autoSize = true;
			thumbImg.maintainAspectRatio = true;
		}
		else
		{
			// some data is invalid hide item
			this.visible = false;
			this.thumbImg.visible = false;
		}
		
		super.draw();
   }
} 