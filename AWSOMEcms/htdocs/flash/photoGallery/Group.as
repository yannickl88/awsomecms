package photoGallery 
{
	import flash.display.MovieClip;
	
	/**
	 * Group of images for the image gallery
	 * 
	 * @author Yannick de Lange
	 */
	public class Group extends MovieClip
	{
		private var titleImage:ImgContainer;
		private var images:Array;
		private var cols:Array;
		private var resizeRatio:Number;
		
		public var title:String;
		
		public function Group(title:String, titleImage:ImgContainer, resizeRatio:Number = 1):void
		{
			this.titleImage = titleImage;
			this.titleImage.x = 10;
			this.titleImage.y = 10;
			
			this.title = title;
			
			this.titleImage.resizeRatio = this.resizeRatio;
			this.titleImage.resize();
			
			this.images = new Array();
			this.cols = new Array();
			
			this.titleImage.group = this;
			this.addChild(this.titleImage);
			this.setResizeRatio(resizeRatio);
		}
		
		public function setResizeRatio(ratio:Number):void
		{
			this.resizeRatio = ratio;
			this.update();
		}
		
		private function update():void
		{
			this.titleImage.resizeRatio = this.resizeRatio;
			this.titleImage.resize();
			
			for each(var image:ImgContainer in this.images)
			{
				image.resizeRatio = this.resizeRatio;
				image.resize();
				
				image.x = this.titleImage.x + (ImgSizes.SIZE_HEADER.width + 10) * this.resizeRatio + (((ImgSizes.SIZE_LARGE.width + 10 ) * this.resizeRatio) * image.position);
			
				if (image.type == ImgContainer.SMALL && image.row == 1)
				{
					image.y = this.titleImage.y + (ImgSizes.SIZE_SMALL.height + 10) * this.resizeRatio;
				}
				else
				{
					image.y = this.titleImage.y;
				}
			}
		}
		
		public function addImage(image:ImgContainer):void
		{
			image.resizeRatio = this.resizeRatio;
			image.resize();
			image.group = this;
			
			//is the image large?
			if (image.type == ImgContainer.LARGE)
			{
				this.cols.push(1);
				image.position = this.cols.length - 1;
			}
			else if (image.type == ImgContainer.SMALL)
			{
				//check if there is a space free
				var freeSpace:Number = -1;
				
				for (var key:String in this.cols)
				{
					if (this.cols[key] == 0.5)
					{
						freeSpace = parseFloat(key);
						break;
					}
				}
				
				if (freeSpace > -1)
				{
					this.cols[key] = 1;
					image.position = parseFloat(key);
					image.row = 1;
				}
				else
				{
					this.cols.push(0.5);
					image.position = this.cols.length - 1;
					image.row = 0;
				}
			}
			
			this.images.push(image);
			this.addChild(image);
			this.update();
		}
		
		public function getWidth():int
		{
			var totalWidth:int = 0;
			
			totalWidth += (ImgSizes.SIZE_HEADER.width * this.resizeRatio)
			totalWidth += ((ImgSizes.SIZE_SMALL.width + 10) * this.resizeRatio) * this.cols.length;
			
			return totalWidth;
		}
		
		public function getImages():Array
		{
			return this.images;
		}
	}
	
}