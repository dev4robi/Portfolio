// Init
$(document).ready(function(){
    // Container Fade In
    $('.container-fluid').hide().fadeIn(500);

    // Add Total Chart
    addSkillbar('#div_total_chart_row', 'total_chart', false);
    updateSkillbarData('#total_chart', '/statistics/all');

    // Attach Search Button Event
    $('#button_search').click(function(){
		var updateDelay = 0; // If 'skillbar_array.length' != zero, must over 500

		if (skillbar_array.length > 0) {
			while (skillbar_array.length > 0) {
				closeSkillbar(skillbar_array.shift());
			}
			skillbar_cnt = 0;
			updateDelay = 500;
		}
		
		setTimeout(function(){
			updateSkillbarData(null, 'statistics/name/' + $('#input_search').val());
		}, updateDelay);
    });
});