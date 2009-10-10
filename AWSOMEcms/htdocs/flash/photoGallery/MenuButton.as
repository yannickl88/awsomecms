package photoGallery 
{
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	/**
	 * ...
	 * @author Yannick de Lange
	 */
	public class MenuButton extends MovieClip
	{
		//elements
		public var clickme:SimpleButton;
		public var bg:MovieClip;
		public var title:TextField;
		
		//properties
		private var group:Group;
		private var panel:PhotoPanel;
		
		public function MenuButton(group:Group, panel:PhotoPanel):void
		{
			this.group = group;
			this.panel = panel;
			
			this.title.autoSize = TextFieldAutoSize.LEFT;
			this.title.wordWrap = false;
			this.title.text = group.title;
			
			this.bg.height = this.clickme.height = 30;
			this.bg.width = this.clickme.width = this.getWidth();
			this.bg.gotoAndStop(1);
			
			this.clickme.addEventListener(MouseEvent.CLICK, this.handleMouse);
			this.clickme.addEventListener(MouseEvent.MOUSE_OUT, this.handleMouse);
			this.clickme.addEventListener(MouseEvent.MOUSE_OVER, this.handleMouse);
		}
		
		public function getWidth():int
		{
			return this.title.textWidth  + 10;
		}
		
		private function handleMouse(e:MouseEvent):void
		{
			if (e.type == MouseEvent.CLICK)
			{
				this.panel.scrollTo(this.group);
			}
			else if (e.type == MouseEvent.MOUSE_OVER)
			{
				this.bg.gotoAndStop(2);
			}
			else if (e.type == MouseEvent.MOUSE_OUT)
			{
				this.bg.gotoAndStop(1);
			}
		}
	}
	
}