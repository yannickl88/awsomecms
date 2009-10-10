package photoGallery 
{
	import flash.display.Bitmap;
	import flash.display.DisplayObject;
	import flash.display.Loader;
	import flash.display.LoaderInfo;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	
	/**
	 * Wrapper around all the images, includes loading etc.
	 * 
	 * @author Yannick de Lange
	 */
	public class ImgContainer extends MovieClip
	{
		//constants
		public static const HEADER:int =	1;
		public static const SMALL:int =		2;
		public static const LARGE:int =		3;
		
		//elements
		public var loader:MovieClip;
		public var maskObject:MovieClip;
		public var clickme:SimpleButton;
		
		//properties
		private var panel:PhotoPanel;
		private var title:String;
		private var image:String;
		private var imageObj:DisplayObject;
		public var group:Group;
		public var type:int;
		public var resizeRatio:Number = 1;
		public var position:Number = -1;
		public var row:int = 0;
		
		public function ImgContainer(image:String, title:String, panel:PhotoPanel, type:int = ImgContainer.LARGE):void
		{
			this.panel = panel;
			this.image = image;
			this.type = type;
			this.title = title;
			
			//let's load the image
			var loader:Loader = new Loader();
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, this.handleLoad);
			loader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR, trace);
			loader.load(new URLRequest(this.image));
			
			this.clickme.addEventListener(MouseEvent.CLICK, this.handleMouse);
		}
		
		private function handleLoad(e:Event):void
		{
			this.loader.visible = false;
			
			var loader:Loader = LoaderInfo(e.currentTarget).loader;
			var bitmap = Bitmap(loader.content);
			bitmap.mask = this.maskObject;
			bitmap.smoothing = true;
			
			this.imageObj = bitmap;
			
			this.addChildAt(bitmap, 0);
			
			this.resize();
		}
		
		public function resize():void
		{
			switch(this.type)
			{
				case ImgContainer.HEADER:
					this.maskObject.width = this.clickme.width = ImgSizes.SIZE_HEADER.width * this.resizeRatio;
					this.maskObject.height = this.clickme.height = ImgSizes.SIZE_HEADER.height * this.resizeRatio;
					break;
				case ImgContainer.LARGE:
					this.maskObject.width = this.clickme.width = ImgSizes.SIZE_LARGE.width * this.resizeRatio;
					this.maskObject.height = this.clickme.height = ImgSizes.SIZE_LARGE.height * this.resizeRatio;
					break;
				case ImgContainer.SMALL:
					this.maskObject.width = this.clickme.width = ImgSizes.SIZE_SMALL.width * this.resizeRatio;
					this.maskObject.height = this.clickme.height = ImgSizes.SIZE_SMALL.height * this.resizeRatio;
					break;
			}
			
			//position the loader
			this.loader.x = this.maskObject.width / 2;
			this.loader.y = this.maskObject.height / 2;
			
			if (imageObj)
			{
				//resize the image
				if (this.imageObj.width > 0)
				{
					var imageRatio = this.imageObj.height / this.imageObj.width;
					
					if (this.imageObj.height > this.imageObj.width)
					{
						trace(this.image + "HEIGHT");
						
						this.imageObj.height = this.maskObject.height;
						this.imageObj.width = this.imageObj.height / imageRatio;
						
						if (this.imageObj.width < this.maskObject.width)
						{
							this.imageObj.width = this.maskObject.width;
							this.imageObj.height = this.imageObj.width * imageRatio;
						}
					}
					else
					{
						trace(this.image + "WIDTH");
						
						this.imageObj.width = this.maskObject.width;
						this.imageObj.height = this.imageObj.width * imageRatio;
						
						if (this.imageObj.height < this.maskObject.height)
						{
							this.imageObj.height = this.maskObject.height;
							this.imageObj.width = this.imageObj.height / imageRatio;
						}
					}
				}
				
				//position the image
				this.imageObj.x = (this.imageObj.width / -2) + (this.maskObject.width / 2);
				this.imageObj.y = (this.imageObj.height / -2) + (this.maskObject.height / 2);
			}
		}
		
		private function handleMouse(e:MouseEvent):void
		{
			if (this.image)
			{
				this.panel.open(this);
			}
		}
		
		public function getURL():String
		{
			return this.image;
		}
		
		public function getTitle():String
		{
			return this.title;
		}
	}
}