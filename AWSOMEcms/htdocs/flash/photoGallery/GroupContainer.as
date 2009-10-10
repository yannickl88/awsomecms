package photoGallery 
{
	import flash.display.MovieClip;
	
	/**
	 * Container object for all groups with extra width calculation logic
	 * 
	 * @author Yannick de Lange
	 */
	public class GroupContainer extends MovieClip
	{
		public function getWidth():int
		{
			var totalWidth:int = 0;
			
			for (var i:int = 0; i < this.numChildren; i++)
			{
				var child:Group = Group(this.getChildAt(i));
				
				totalWidth += child.getWidth();
			}
			
			return totalWidth;
		}
	}
	
}