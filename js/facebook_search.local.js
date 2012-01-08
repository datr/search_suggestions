Drupal.behaviors.facebook_search = function (context) {
	$('.facebook_search_submit').remove();
	$('.facebook_search_form').submit(function() {
	  if (!$('.ac_results').is(':visible')) {
			return false;
	  }
	});
	$('.facebook_search').attr('size', 27).autocomplete(Drupal.settings.facebook_search.data, {
    selectFirst: false,
    matchContains: true,
  	matchCase: false,
    scroll: Drupal.settings.facebook_search.scroll,
    scrollHeight: Drupal.settings.facebook_search.scroll_height,
    formatMatch: function(row, i, max) {
    	return row.title;
    }, 
    formatItem: function(row, i, max) {
    	return row.title;
    },
    formatResult: function(row) {
    	return row.url;
    } 
  }).result(function(event, item) {
  	if (item.url != undefined) {
      location.href = Drupal.settings.basePath + item.url;
    }
  });
};
