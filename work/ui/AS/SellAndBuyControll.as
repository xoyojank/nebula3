import flash.external.ExternalInterface;

import gfx.controls.ListItemRenderer;
import gfx.controls.ScrollingList;
import gfx.controls.Label;
import gfx.data.DataProvider;

class AS.SellAndBuyControll
{
	private var playerScroll : ScrollingList;
	private var traderScroll : ScrollingList;
	public var playerIndex : Number;
	private var traderIndex : Number;
	
	//------------------------------------------------------------------------------
	/** Constructor
	*/
	function SellAndBuyControll(playerScroll : ScrollingList, traderScroll : ScrollingList)
	{
		this.playerIndex = -1;
		this.traderIndex = -1;
		trace("AS: SellAndBuyControll::Constructor()");
		this.playerScroll = playerScroll;
		this.traderScroll = traderScroll;
		this.playerScroll.addEventListener("change", this, "PlayerScrollIndexChanged");
		this.traderScroll.addEventListener("change", this, "TraderScrollIndexChanged");
	/*	this.playerScroll.dataProvider = [{ _label: "test", _imagePath: "test.dds" }, 
										  { _label: "test2", _imagePath: "test.dds" } ];
		ExternalInterface.call("TestControl.updateData", []);*/

	}
	
	//------------------------------------------------------------------------------
	/** Callback: Called when Buy butten is clicked
	*/
	function SellButtonClicked() : Void
	{
		trace("AS: Sell button clicked");
		ExternalInterface.call("TestControl.sell", []);
		ExternalInterface.call("TestControl.updateData", []);
	}
		
	//------------------------------------------------------------------------------
	/** Callback: Called when Buy butten is clicked
	*/
	function BuyButtonClicked() : Void
	{
		trace("AS: Buy button clicked");
		ExternalInterface.call("TestControl.buy", []);
		ExternalInterface.call("TestControl.updateData", []);
	}
		
	//------------------------------------------------------------------------------
	/** Callback: Called when player scroll item is clicked
	*/
	function PlayerScrollIndexChanged() : Void
	{
		trace("AS: PlayerScrollIndexChanged");
		if (this.playerIndex != this.playerScroll.selectedIndex)
		{
			this.playerIndex = this.playerScroll.selectedIndex;
			ExternalInterface.call("TestControl.playerScrollIndexChanged", []);
		}
	}
	
	//------------------------------------------------------------------------------
	/** Callback: Called when player scroll item is clicked
	*/
	function TraderScrollIndexChanged() : Void
	{
		trace("AS: TraderScrollIndexChanged");
		if (this.traderIndex != this.traderScroll.selectedIndex)
		{
			this.traderIndex = this.traderScroll.selectedIndex;
			ExternalInterface.call("TestControl.traderScrollIndexChanged", []);
		}
	}
}