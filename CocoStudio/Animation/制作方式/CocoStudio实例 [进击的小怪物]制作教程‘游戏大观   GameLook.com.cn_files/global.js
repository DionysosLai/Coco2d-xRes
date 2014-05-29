jQuery(document).ready(function(){
		// 	Tab Sidebar
		$('#tab-sidebar').each(function() {
			$(this).prepend('<div class="tab-widget-menu clear"><ul></ul></div>');
			$(this).find('.widget').each(function(n) {
				$('.tab-widget-menu ul').append('<li class="tab' + (n+1) + '">' + $(this).children('h3').html() + '</li>');												
			});
		});
		$('#tab-sidebar .widget').hide();
		$('#tab-sidebar .widget-title').remove();
		$('#tab-sidebar .widget:first').show();
		$('.tab-widget-menu ul li:first').addClass('selected');
		$('.tab-widget-menu ul li').click(function(){ 
			$('.tab-widget-menu ul li').removeClass('selected');
			$(this).addClass('selected');
			$('#tab-sidebar .widget').hide();
			$('#tab-sidebar .widget').eq($('.tab-widget-menu ul li').index(this)).fadeIn(500);
		});
		
		// 	Remove right border from last-child of catnav
		$('#tab-sidebar .widget ul li:first-child').css('border-top','0');
		 
		
		// Layout Switcher for post list
		if($.cookie('MODE_SWITCHER') == 'grid-post') {
			$('.display').addClass('display-grid');
			$('.list-content .hentry').addClass($.cookie('MODE_SWITCHER'));
		}
		$('.display').click(function(){
			$(this).toggleClass('display-grid');
			$('.list-content .hentry').toggleClass('grid-post');
			if($('.list-content .hentry').hasClass('grid-post')) {
				var postmode = 'grid-post';
			} else {
				var postmode = '';
			}
			$.cookie('MODE_SWITCHER',postmode, { path: '/', expires: 10000 });
			return false;
		});
		
		// Carousel
		$(".carousel-posts").jCarouselLite({
			btnNext: ".pc-next",
			btnPrev: ".pc-prev",
			visible: 4,
			scroll: 4,
			speed: 200
		});

		
		// jQuery Superfish menu
		$('ul.sf-menu').superfish(); 
		
});