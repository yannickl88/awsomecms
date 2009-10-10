package photoGallery 
{
	import fl.transitions.Tween;
	import fl.transitions.easing.Regular;
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	
	/**
	 * The main scrollable pannel for the photos
	 * 
	 * @author Yannick de Lange
	 */
	public class PhotoPanel extends MovieClip
	{
		//constants
		public static const NORMAL:int =	1;
		public static const SCROLLING:int =	2;
		
		//elements
		public var maskObject:DisplayObject;
		public var bg:DisplayObject;
		public var arrowLeft:DisplayObject;
		public var arrowRight:DisplayObject;
		
		//properties
		private var groups:Array;
		private var menus:Array;
		private var acc:Number = 0;
		private var groupsContainer:GroupContainer;
		private var state = PhotoPanel.NORMAL;
		
		public function PhotoPanel():void
		{
			this.groups = new Array();
			this.menus = new Array();
			this.addEventListener(Event.ADDED_TO_STAGE, this.handelAdd);
			this.update();
			this.groupsContainer = new GroupContainer();
			this.groupsContainer.mask = this.maskObject;
			this.addChildAt(groupsContainer, this.getChildIndex(this.bg) + 1);
			
			this.arrowLeft.addEventListener(MouseEvent.MOUSE_DOWN, this.handleMouseOver);
			this.arrowLeft.addEventListener(MouseEvent.MOUSE_UP, this.handleMouseOut);
			this.arrowRight.addEventListener(MouseEvent.MOUSE_DOWN, this.handleMouseOver);
			this.arrowRight.addEventListener(MouseEvent.MOUSE_UP, this.handleMouseOut);
			this.addEventListener(Event.ENTER_FRAME, this.scroll);
		}
		
		public function addGroup(group:Group):void
		{
			this.groups.push(group);
			this.groupsContainer.addChild(group);
			
			//create the menu item
			var button = new MenuButton(group, this);
			button.x = 0;
			
			for each(var menuButton:MenuButton in this.menus)
			{
				button.x += menuButton.getWidth() + 10;
			}
			
			button.y = -40;
			this.menus.push(button);
			this.addChild(button);
			
			this.update();
		}
		
		private function handelAdd(e:Event):void
		{
			this.removeEventListener(Event.ADDED_TO_STAGE, this.handelAdd);
			this.update();
			
			this.stage.addEventListener(Event.RESIZE, this.update);
		}
		
		private function scroll(e:Event):void
		{
			if (this.state == PhotoPanel.NORMAL)
			{
				var maxScroll:int = 25;
				var ratio = (this.maskObject.height - 20) / ImgSizes.SIZE_HEADER.height;
				var groupsWidth = this.groupsContainer.getWidth();
				
				if (groupsWidth > this.bg.width)
				{
					if (this.groupsContainer.x + this.acc < 0 && this.groupsContainer.x + this.acc > (groupsWidth * -1) + this.bg.width - (20 * ratio))
					{
						this.groupsContainer.x += this.acc;
						
						if (this.acc < maxScroll && this.acc > -1 * maxScroll)
						{
							this.acc *= 1.2;
						}
						
						if(this.acc > maxScroll)
						{
							this.acc = maxScroll;
						}
						else if(this.acc < -1 * maxScroll)
						{
							this.acc = -1 * maxScroll;
						}
					}
					else
					{
						if (this.groupsContainer.x + this.acc > 0)
						{
							this.groupsContainer.x = 0;
						}
						else if (this.groupsContainer.x + this.acc < (groupsWidth * -1) + this.bg.width - (20 * ratio))
						{
							this.groupsContainer.x = (groupsWidth * -1) + this.bg.width - (20 * ratio);
						}
					}
				}
				else
				{
					this.groupsContainer.x = 0;
				}
			}
		}
		
		private function handleMouseOver(e:MouseEvent):void
		{
			if (this.acc == 0 && this.state == PhotoPanel.NORMAL)
			{
				var arrow:SimpleButton = SimpleButton(e.currentTarget);
				if (arrow == this.arrowLeft)
				{
					this.acc = 1;
				}
				else if (arrow == this.arrowRight)
				{
					this.acc = -1;
				}
			}
		}
		
		private function handleMouseOut(e:MouseEvent):void
		{
			this.acc = 0;
		}
		
		public function update(e:Event = null):void
		{
			this.x = 50;
			this.y = 50;
			
			this.maskObject.x = 10;
			this.maskObject.width = this.stage.stageWidth - 120;
			this.bg.width = this.stage.stageWidth - 100;
			this.maskObject.height = this.bg.height = this.stage.stageHeight - this.y - 10;
			
			this.arrowLeft.x = -10;
			this.arrowLeft.y = 0;
			this.arrowLeft.height = this.bg.height;
			this.arrowRight.x = this.bg.width + 10;
			this.arrowRight.y = 0;
			this.arrowRight.height = this.bg.height;
			
			var currentWidth = 0;
			
			for each(var group:Group in this.groups)
			{
				group.x = currentWidth;
				group.setResizeRatio((this.maskObject.height - 20) / ImgSizes.SIZE_HEADER.height);
				
				currentWidth += group.getWidth() + (10 * (this.maskObject.height - 20) / ImgSizes.SIZE_HEADER.height);
			}
		}
		
		public function scrollTo(group:Group)
		{
			var groupsWidth:int = this.groupsContainer.getWidth();
			
			if (groupsWidth > this.bg.width)
			{
				var scrollingTo:Number = group.x * -1;
				var ratio:Number = (this.maskObject.height - 20) / ImgSizes.SIZE_HEADER.height;
				
				if (scrollingTo < (groupsWidth * -1) + this.bg.width - (20 * ratio))
				{
					scrollingTo = (groupsWidth * -1) + this.bg.width - (20 * ratio);
				}
				
				var tween:Tween = new Tween(this.groupsContainer, 'x', Regular.easeInOut, this.groupsContainer.x, scrollingTo, 1, true);
			}
		}
		
		public function open(image:ImgContainer):void
		{
			var images:Array = image.group.getImages();
			var imageArray:Array = new Array();
			var firstImage:ImgContainer;
			
			//header image needs to open the first image, not the header image
			if (image.type == ImgContainer.HEADER)
			{
				firstImage = images[0];
			}
			else
			{
				firstImage = image;
			}
			
			for each(var img in images)
			{
				var imageDict = new Object();
				
				imageDict.url = img.getURL();
				imageDict.title = img.getTitle();
				
				imageArray.push(imageDict);
			}
			
			ExternalInterface.call('lightbox', imageArray, firstImage.getURL());
		}
	}
	
}