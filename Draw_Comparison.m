close all; addpath('data')

%%

load sim_white_box_nom

draw_comparison_fcn(time, state_sim, one_step_pred, controls_MPC, 'g');


%%

load sim_cont_grey_box

draw_comparison_fcn(time, state_sim, one_step_pred, controls_MPC,'b');

%%

load sim_cont_black_box

draw_comparison_fcn(time, state_sim, one_step_pred, controls_MPC,'r');

%%

load sim_white_box_corr

draw_comparison_fcn(time, state_sim, one_step_pred, controls_MPC,'k');

%%

fig1 = figure(1);
subplot(311)
xlim([0 3])
subplot(312)
xlim([0 3])
subplot(313)
xlim([0 3])
legend('Nominal model','Grey box model', 'Black box model','Correct analytical model')
saveas(fig1,'data/swingup_pos','epsc')

fig2 = figure(2)
subplot(211)
xlim([0 3])
subplot(212)
xlim([0 3])
legend('Nominal model','Grey box model', 'Black box model','Correct analytical model')
saveas(fig2,'data/swingup_diff','epsc')


