package  
{
	import flash.display.LoaderInfo;
	import flash.display.MovieClip;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import photoGallery.Group;
	import photoGallery.ImgContainer;
	import photoGallery.PhotoPanel;
	
	import com.serialization.json.JSON;

	public class Main extends MovieClip
	{
		public var mainPanel:PhotoPanel;
		
		private var dataURL:String = '';
		
		public function Main() 
		{
			this.stage.align = StageAlign.TOP_LEFT;
			this.stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var paramObj:Object = LoaderInfo(this.root.loaderInfo).parameters;
			
			//config items
			this.dataURL = paramObj.request;
			
			//loading
			var request:URLRequest = new URLRequest(this.dataURL + '/groups');
			var loader:URLLoader = new URLLoader(request);
			loader.addEventListener(Event.COMPLETE, this.handleDataLoad);
			
			/* debug
			var group:Group = new Group('Hyves', new ImgContainer('img/hyvesbg.jpg', null, mainPanel, ImgContainer.HEADER));
			
			group.addImage(new ImgContainer('img/hyvesbg2.jpg', 'img/hyvesbg2.jpg', mainPanel, ImgContainer.LARGE));
			group.addImage(new ImgContainer('img/hyvesbg3.jpg', 'img/hyvesbg3.jpg', mainPanel, ImgContainer.SMALL));
			group.addImage(new ImgContainer('img/hyvesbg4.jpg', 'img/hyvesbg4.jpg', mainPanel, ImgContainer.SMALL));
			
			this.mainPanel.addGroup(group);
			
			var group2:Group = new Group('Hyves', new ImgContainer('img/hyvesbg.jpg', null, mainPanel, ImgContainer.HEADER));
			
			group2.addImage(new ImgContainer('img/hyvesbg2.jpg', 'img/hyvesbg2.jpg', mainPanel, ImgContainer.LARGE));
			group2.addImage(new ImgContainer('img/hyvesbg3.jpg', 'img/hyvesbg3.jpg', mainPanel, ImgContainer.SMALL));
			group2.addImage(new ImgContainer('img/hyvesbg4.jpg', 'img/hyvesbg4.jpg', mainPanel, ImgContainer.SMALL));
			
			this.mainPanel.addGroup(group2);
			*/
		}
		
		private function handleDataLoad(event:Event)
		{
			var loader:URLLoader = URLLoader(event.target);
			var data:Object = JSON.deserialize(loader.data);
			
            for each(var groupData:Object in data)
			{
				var group:Group = new Group(groupData.group_title, new ImgContainer(groupData.image_url, null, mainPanel, ImgContainer.HEADER));
				
				for each(var imageData:Object in groupData.images)
				{
					var size:int = ImgContainer.SMALL;
					
					if (imageData.image_size == 2)
					{
						size = ImgContainer.LARGE;
					}
					group.addImage(new ImgContainer(imageData.image_url, imageData.image_title, mainPanel, size));
				}
				
				this.mainPanel.addGroup(group);
			}
		}
	}
	
}