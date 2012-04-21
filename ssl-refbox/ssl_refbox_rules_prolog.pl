%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%															%%
%%		Definitions and helper predicates 									%%
%%															%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Position, e.g. for saving freekick position
:- dynamic position/4.
set_freekick_pos(Pos_x,Pos_y,Pos_z) :- 
	not(position('freekick_pos',_,_,_)) , 
	assert(position('freekick_pos',Pos_x,Pos_y,Pos_z)) , 
	!.
set_freekick_pos(Pos_x,Pos_y,Pos_z) :- 
	retract(position('freekick_pos',_,_,_)) , 
	assert(position('freekick_pos',Pos_x,Pos_y,Pos_z)).
get_freekick_pos(Pos_x,Pos_y,Pos_z) :- 
	position('freekick_pos',Pos_x,Pos_y,Pos_z).

%Field
:- dynamic field/9.
define_field(Field_width,Field_height,Goal_width,Goal_Depth,Goal_Height,Defense_radius,Defense_line,Penalty_mark) :- 
	not(field('field',_,_,_,_,_,_,_,_)) , 
	assert(field('field',Field_width,Field_height,Goal_width,Goal_Depth,Goal_Height,Defense_radius,Defense_line,Penalty_mark)).

%Ball
set_ball_stuff(Pos_x,Pos_y,Pos_z,Speed_x,Speed_y,Speed_z,Last_touched_team,Last_touched_id,Status) :- 
	set_ball_location(Pos_x,Pos_y,Pos_z,Speed_x,Speed_y,Speed_z) , 
	set_ball_status(Last_touched_team,Last_touched_id,Status).

%Ballvariable
:- dynamic ball_location/7.
set_ball_location(Pos_x,Pos_y,Pos_z,Speed_x,Speed_y,Speed_z) :- 
	not(ball_location('ball',_,_,_,_,_,_))     , 
	assert(ball_location('ball',Pos_x,Pos_y,Pos_z,Speed_x,Speed_y,Speed_z)) , 
	!.
set_ball_location(Pos_x,Pos_y,Pos_z,Speed_x,Speed_y,Speed_z) :- 
	retract(ball_location('ball',_,_,_,_,_,_)) , 
	assert(ball_location('ball',Pos_x,Pos_y,Pos_z,Speed_x,Speed_y,Speed_z)).

%Ball status
:- dynamic ball_status/5.
%Variable für Abseits
:- dynamic check_offside/1.
%Neuer Last_touched, Status += 1
set_ball_status(Ltt_1,Ltid_1,Status) :- 
	ball_status('ball',Ltt_2,Ltid_2,_,X) , 
	not((Ltt_1 =:= Ltt_2 , Ltid_1 =:= Ltid_2)) , 
	Y is X+1 , 
	retract(ball_status('ball',_,_,_,_)) , 
	assert(ball_status('ball',Ltt_1,Ltid_1,Status,Y)) , 
	retract(check_offside(_)) , 
	assert(check_offside(1)).
%Nur neuer Status
set_ball_status(Ltt,Ltid,Status_1) :- 
	ball_status('ball',_,_,Status_2,X) , 
	Status_1 =\= Status_2 , 
	retract(ball_status('ball',_,_,_,_)) , 
	assert(ball_status('ball',Ltt,Ltid,Status_1,X)) , 
	retract(check_offside(_)) , 
	assert(check_offside(1)).
%Nur den Touch setzen, bei neuen Play_states
set_ball_status_touch(Touch) :- 
	not(ball_status('ball',_,_,_,_)) , 
	assert(ball_status('ball',-3,-3,-3,Touch)) , 
	!.
set_ball_status_touch(Touch) :- 
	retract(ball_status('ball',Team,ID,Status,_)) , 
	assert(ball_status('ball',Team,ID,Status,Touch)).
set_touch(New) :- 
	((New =:= 6) ; (New =:= 12)) , 
	set_ball_status_touch(-1) , 
	!.
set_touch(New) :- 
	New =\= 6 , 
	New =\= 12 , 
	set_ball_status_touch(0).
reset_touch :- 
	play_state('local_play_state',Next) , 
	set_touch(Next) , 
	!.
get_ball_status(Ltt,Ltid,Status,Touch) :- 
	ball_status('ball',Ltt,Ltid,Status,Touch).

%Robotervariable
:- dynamic roboter/7.
set_robot(Team,ID,Pos_x,Pos_y,Speed_x,Speed_y,Visible) :- 
	not(roboter(Team,ID,_,_,_,_,_))    , 
	assert(roboter(Team,ID,Pos_x,Pos_y,Speed_x,Speed_y,Visible)) , 
	!.
set_robot(Team,ID,Pos_x,Pos_y,Speed_x,Speed_y,Visible) :- 
	retract(roboter(Team,ID,_,_,_,_,_)), 
	assert(roboter(Team,ID,Pos_x,Pos_y,Speed_x,Speed_y,Visible)).

%Roboter who breaks the rule
:- dynamic rule_breaker/3.
set_rule_breaker(Team,ID) :- 
	not(rule_breaker('rule_breaker',_,_))     , 
	assert(rule_breaker('rule_breaker',Team,ID)) , 
	!.
set_rule_breaker(Team,ID) :- 
	retract(rule_breaker('rule_breaker',_,_)) , 
	assert(rule_breaker('rule_breaker',Team,ID)).
get_rule_breaker(Team,ID) :- 
	rule_breaker('rule_breaker',Team,ID).

%timestamp
:- dynamic timestamp/2.
%aktueller Timestamp gesetzt von außen
set_timestamp(Timestamp) :- 
	not(timestamp('timestamp',_)) , 
	assert(timestamp('timestamp',Timestamp)) , 
	!.
set_timestamp(Timestamp) :- 
	retract(timestamp('timestamp',_)) , 
	assert(timestamp('timestamp',Timestamp)).
get_timestamp(Timestamp) :- 
	timestamp('timestamp',Timestamp).
%drei Sekunden zwischen Ansage des "before"-Spielstandes und Start
set_three_seconds :- 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('three_seconds',_)) , 
	assert(timestamp('three_seconds',Timestamp)) , 
	!.
set_three_seconds :- 
	timestamp('timestamp',Timestamp) , 
	retract(timestamp('three_seconds',_)) , 
	assert(timestamp('three_seconds',Timestamp)).
reset_three_seconds :- 
	retract(timestamp('three_seconds',_)) , 
	assert(timestamp('three_seconds',0)).
get_three_seconds(Timestamp) :- 
	timestamp('three_seconds',Timestamp).
%Zehn Sekunden zwischen Freigabe des Freistoßes und Ausführung
set_ten_seconds :- 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('ten_seconds',_)) , 
	assert(timestamp('ten_seconds',Timestamp)) , 
	!.
set_ten_seconds :- 
	timestamp('timestamp',Timestamp) , 
	retract(timestamp('ten_seconds',_)) , 
	assert(timestamp('ten_seconds',Timestamp)).
reset_ten_seconds :- 
	retract(timestamp('ten_seconds',_)) , 
	assert(timestamp('ten_seconds',0)).
get_ten_seconds(Timestamp) :- 
	timestamp('ten_seconds',Timestamp).
%Echtspielzeit, also nur Zeit während running, Der Anfang
set_real_play_time_start :- 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('real_play_time_start',_)) , 
	assert(timestamp('real_play_time_start',Timestamp)) , 
	!.
set_real_play_time_start :- 
	timestamp('timestamp',Timestamp) , 
	retract(timestamp('real_play_time_start',_)) , 
	assert(timestamp('real_play_time_start',Timestamp)).
get_real_play_time_start(Timestamp) :- 
	timestamp('real_play_time_start',Timestamp).
%Echtspielzeit, also nur Zeit während running, Aufaddiert
set_real_play_time_end :- 
	timestamp('real_play_time_start',Start) , 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('real_play_time_total',_)) , 
	New is Timestamp-Start , 
	assert(timestamp('real_play_time_total',New)) , 
	!.
set_real_play_time_end :- 
	timestamp('real_play_time_start',Start) , 
	timestamp('timestamp',Timestamp) , 
	timestamp('real_play_time_total',Old) , 
	New is Old+(Timestamp-Start) , 
	retract(timestamp('real_play_time_total',_)) , 
	assert(timestamp('real_play_time_total',New)).
get_real_play_time_total(Timediff) :- 
	timestamp('real_play_time_total',Timediff).
%Zeit vom ersten Anpfiff
set_total_play_time_start :- 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('total_play_time_start',_)) , 
	assert(timestamp('total_play_time_start',Timestamp)) , 
	!.
set_total_play_time_start :- 
	timestamp('timestamp',Timestamp) , 
	retract(timestamp('total_play_time_start',_)) , 
	assert(timestamp('total_play_time_start',Timestamp)).
reset_total_play_time_start :- 
	retract(timestamp('total_play_time_start',_)) , 
	assert(timestamp('total_play_time_start',0)).
get_total_play_time_start(Timestamp) :- 
	timestamp('total_play_time_start',Timestamp).
%Totale Spielzeit, jede Halbzeit einzeln
set_total_play_time_end :- 
	timestamp('total_play_time_start',Start) , 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('total_play_time_total',_)) , 
	New is Timestamp-Start , 
	assert(timestamp('total_play_time_total',New)) , 
	!.
set_total_play_time_end :- 
	timestamp('total_play_time_start',Start) , 
	timestamp('timestamp',Timestamp) , 
	timestamp('total_play_time_total',Old) , 
	New is Old+(Timestamp-Start) , 
	retract(timestamp('total_play_time_total',_)) , 
	assert(timestamp('total_play_time_total',New)).
get_total_play_time_total(Sum) :- 
	timestamp('total_play_time_total',Total) , 
	timestamp('total_play_time_start',Start) , 
	timestamp('timestamp',Now) , 
	Sum is Total+(Now-Start).
%Timeout gelb Anfang
set_timeout_start_yellow :- 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('timeout_start_yellow',_)) , 
	assert(timestamp('timeout_start_yellow',Timestamp)) , 
	!.
set_timeout_start_yellow :- 
	timestamp('timestamp',Timestamp) , 
	retract(timestamp('timeout_start_yellow',_)) , 
	assert(timestamp('timeout_start_yellow',Timestamp)).
get_timeout_start_yellow(Timestamp) :- 
	timestamp('timeout_start_yellow',Timestamp).
%Timeout blau Anfang
set_timeout_start_blue :- 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('timeout_start_blue',_)) , 
	assert(timestamp('timeout_start_blue',Timestamp)) , 
	!.
set_timeout_start_blue :- 
	timestamp('timestamp',Timestamp) , 
	retract(timestamp('timeout_start_blue',_)) , 
	assert(timestamp('timeout_start_blue',Timestamp)).
get_timeout_start_blue(Timestamp) :- 
	timestamp('timeout_start_blue',Timestamp).
%timeout gelb Übersicht
set_yellow_timeout_end :- 
	timestamp('timeout_start_yellow',Start) , 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('timeout_time_yellow',_)) , 
	New is Timestamp-Start , 
	assert(timestamp('timeout_time_yellow',New)) , 
	!.
set_yellow_timeout_end :- 
	timestamp('timeout_start_yellow',Start) , 
	timestamp('timestamp',Timestamp) , 
	timestamp('timeout_time_yellow',Old) , 
	New is Old+(Timestamp-Start) , 
	retract(timestamp('timeout_time_yellow',_)) , 
	assert(timestamp('timeout_time_yellow',New)).
get_yellow_timeout_total(Timediff) :- 
	timestamp('timeout_time_yellow',Timediff).
%timeout blau Übersicht
set_blue_timeout_end :- 
	timestamp('timeout_start_blue',Start) , 
	timestamp('timestamp',Timestamp) , 
	not(timestamp('timeout_time_blue',_)) , 
	New is Timestamp-Start , 
	assert(timestamp('timeout_time_blue',New)) , 
	!.
set_blue_timeout_end :- 
	timestamp('timeout_start_blue',Start) , 
	timestamp('timestamp',Timestamp) , 
	timestamp('timeout_time_blue',Old) , 
	New is Old+(Timestamp-Start) , 
	retract(timestamp('timeout_time_blue',_)) , 
	assert(timestamp('timeout_time_blue',New)).
get_blue_timeout_total(Timediff) :- 
	timestamp('timeout_time_blue',Timediff).

%Timeouts
:- dynamic timeouts/3.

%called from outside
start_yellow_timeout :- 
	timestamp('timeout_start',0) , 
	timeouts('timeouts',Y,B) , 
	set_total_play_time_end ,
	set_local_play_state(1) , 
	set_timeout_start , 
	retract(timeouts('timeouts',_,_)) , 
	New is Y+1 , 
	assert(timeouts('timeouts',New,B)).
start_blue_timeout   :- 
	timestamp('timeout_start',0) , 
	timeouts('timeouts',Y,B) , 
	set_total_play_time_end , 
	set_local_play_state(1) , 
	set_timeout_start , 
	retract(timeouts('timeouts',_,_)) , 
	New is B+1 , 
	assert(timeouts('timeouts',Y,New)).
end_yellow_timeout   :- 
	set_local_play_state(1) , 
	set_timeout_yellow_end, 
	set_total_play_time_start , 
	retract(timestamp('timeout_start_yellow',_)) , 
	assert(timestamp('timeout_start_yellow',0)).
end_blue_timeout     :- 
	set_local_play_state(1) , 
	set_timeout_blue_end , 
	set_total_play_time_start , 
	retract(timestamp('timeout_start_blue',_)) , 
	assert(timestamp('timeout_start_blue',0)).

%play_state: global_play_state, local_play_state, local_next_play_state
:- dynamic play_state/2.
:- dynamic left_team/2.
:- dynamic goalie/3.
:- dynamic result/3.

%called from outside
get_standing(X,Y) :- 
	result('result',X,Y).

%called from outside
end_first_half :- 
	reset_total_play_time_start.

%Gamestate Wechsel aus Running in Stopped
set_next_play_state(New) :- 
	play_state('local_play_state',2) , 
	set_local_next_play_state(New) , 
	set_local_play_state(1) , 
	reset_touch , 
	set_real_play_time_end.

%Init
game_init :- 
	set_local_next_play_state(7) , 
	set_local_play_state(0) , 
	assert(result('result',0,0)) , 
	assert(timestamp('total_play_time_start',0)) , 
	assert(timestamp('total_play_time_total',0)) , 
	assert(timestamp('timeout_start_yellow',0)) , 
	assert(timestamp('timeout_start_blue',0)) , 
	assert(timeouts('timeouts',0,0)) , 
	assert(check_offside(0)) , 
	assert(ball_status('ball',0,0,0,0)).

%Game control
%Es macht Probleme wenn Play_States nicht gleich sind
game_control :- 
	start_game ; set_running.
%nur Running setzen wegen der Probleme
%game_control :- set_running.

%Regeln die auch Sachen ändern
 check_rules(X) :- rule_twentynine , ! , X is 29.
 check_rules(X) :- rule_thirty , ! , X is 30.
 check_rules(X) :- rule_sixteen , ! , X is 16.
 check_rules(X) :- rule_fifteen , ! , X is 15.
 check_rules(X) :- rule_fourteen , ! , X is 14.
 check_rules(X) :- rule_seventeen , ! , X is 17.
% check_rules(X) :- rule_twentythree , ! , X is 23.
 check_rules(X) :- rule_nineteen , ! , X is 19.
%Regeln die nur Warnungen ausgeben
check_rules(X) :- rule_three , ! , X is 3.
check_rules(X) :- rule_twentyeight , ! , X is 28.
check_rules(X) :- rule_twentyseven , ! , X is 27.
check_rules(X) :- rule_eighteen , ! , X is 18.
check_rules(X) :- rule_twentytwo , ! , X is 22.
check_rules(X) :- rule_one , ! , X is 1.
check_rules(X) :- rule_fourtytwo , ! , X is 42.
%check_rules(X) :- rule_twentyfour , ! , X is 24.

%Starten des Spiels (nur wenn der Ball liegt = Regel31 + 21)
%aus stopped in before
start_game :- get_local_play_state(1)  , get_ball_status(_,_,6,_) , not(rule_sixteen) , get_local_next_play_state(Next) , ((Next =:= 7) ; (Next =:= 8) ; (Next =:= 13) ; (Next =:= 14)) , set_local_play_state(Next) , Next_after is (Next-4) , set_local_next_play_state(Next_after) , reset_touch , set_three_seconds.
%aus stopped in freekick
start_game :- get_local_play_state(1)  , get_ball_status(_,_,6,_) , not(rule_sixteen) , get_local_next_play_state(Next) , ((Next =:= 5) ; (Next =:= 6) ; (Next =:= 11) ; (Next =:= 12)) , set_local_play_state(Next) , reset_touch , set_ten_seconds.
%aus stopped in running
start_game :- get_local_play_state(1)  , get_ball_status(_,_,6,_) , not(rule_sixteen) , get_local_next_play_state(2) , set_local_play_state(2).
%aus before in kickoff/penalty
start_game :- get_local_play_state(7)  , get_ball_status(_,_,6,_) , not(rule_fourteen)  , get_timestamp(T1) , get_three_seconds(T2) , T3 is T1-T2 , T3 >= 3000 , set_local_play_state(3) , set_local_next_play_state(7) , reset_touch , set_ten_seconds.
start_game :- get_local_play_state(8)  , get_ball_status(_,_,6,_) , not(rule_seventeen) , get_timestamp(T1) , get_three_seconds(T2) , T3 is T1-T2 , T3 >= 3000 , set_local_play_state(4) , set_local_next_play_state(8) , reset_touch , set_ten_seconds.
start_game :- get_local_play_state(13) , get_ball_status(_,_,6,_) , not(rule_fourteen)  , get_timestamp(T1) , get_three_seconds(T2) , T3 is T1-T2 , T3 >= 3000 , set_local_play_state(9) , set_local_next_play_state(13) , reset_touch , set_ten_seconds.
start_game :- get_local_play_state(14) , get_ball_status(_,_,6,_) , not(rule_seventeen) , get_timestamp(T1) , get_three_seconds(T2) , T3 is T1-T2 , T3 >= 3000 , set_local_play_state(10) , set_local_next_play_state(14) , reset_touch , set_ten_seconds.

%Gamestate Wechsel auf running, Kontrolle ob richtiges Team berührt
%right team touches
set_running :- get_local_play_state(Now) , ball_status('ball',Ltt,_,_,Touch) , ( ((Now =:= (3+(Ltt*6)) ; Now =:= (4+(Ltt*6)) ; Now =:= (5+(Ltt*6)) ) , Touch >= 1) ; (Now =:= (6+(Ltt*6)) , Touch >= 0 ) ) , set_local_play_state(2) , set_real_play_time_start , reset_ten_seconds.
%wrong team touches
set_running :- get_local_play_state(Now) , ball_status('ball',Ltt,_,_,Touch) , ( (( Now =:= (3+(abs(Ltt-1)*6)) ; Now =:= (4+(abs(Ltt-1)*6)) ; Now =:= (5+(abs(Ltt-1)*6)) ) , Touch >= 1 ) ; (Now =:= (6+(abs(Ltt-1)*6)) , Touch >= 0 )) , set_local_play_state(1) , set_local_next_play_state(Now) , reset_ten_seconds.

set_global_play_state(Play_state) :- not(play_state('global_play_state',_)), assert(play_state('global_play_state',Play_state)) , determine_sides(Play_state) , !.
set_global_play_state(Play_state) :- retract(play_state('global_play_state',_)), assert(play_state('global_play_state',Play_state)) , determine_sides(Play_state).
get_global_play_state(Play_state) :- play_state('global_play_state',Play_state).

%Falls ein kickoff und Zeit noch nicht angefangen starte Spielzeit
set_local_play_state_from_outside(Play_state) :- (Play_state =:= 3 ; Play_state =:= 9) , timestamp('total_play_time_start',0) , set_total_play_time_start , set_local_play_state(Play_state) , reset_touch , set_ten_seconds.
%Falls ein before_kickoff den Play_State danach setzen
set_local_play_state_from_outside(Play_state) :- (Play_state =:= 7 ; Play_state =:= 8 ; Play_state =:= 13 ; Play_state =:= 14) ,  Next is (Play_state-4) , set_local_play_state(Play_state) , reset_touch , set_three_seconds , set_local_next_play_state(Next).
%Wenn Wechsel in running oder Wechsel aus running dann reale spielzeit starten/beenden
set_local_play_state_from_outside(New) :- get_local_play_state(Old) , Old =\= 2 , New =:= 2 , set_real_play_time_start , set_local_play_state(New) , reset_touch.
set_local_play_state_from_outside(New) :- get_local_play_state(Old) , Old =:= 2 , New =\= 2 , set_real_play_time_end , set_local_play_state(New) , reset_touch.
%Wenn Wechsel in Freekick die 10 Sekunden starten
set_local_play_state_from_outside(Play_state) :- ((Play_state =:= 3) ; (Play_state =:= 4) ; (Play_state =:= 5) ; (Play_state =:= 6) ; (Play_state =:= 9) ; (Play_state =:= 10) ; (Play_state =:= 11) ; (Play_state =:= 12)) , set_local_play_state(Play_state) , reset_touch , set_ten_seconds.
%Jeder andere Fall
set_local_play_state_from_outside(Play_state) :- set_local_play_state(Play_state) , reset_touch , reset_ten_seconds.

set_local_play_state(Play_state) :- not(play_state('local_play_state',_)), assert(play_state('local_play_state',Play_state)) , reset_touch , !.
set_local_play_state(Play_state) :- retract(play_state('local_play_state',_)), assert(play_state('local_play_state',Play_state)) , reset_touch.
get_local_play_state(Play_state) :- play_state('local_play_state',Play_state).
set_local_next_play_state(Play_state) :- not(play_state('local_next_play_state',_)), assert(play_state('local_next_play_state',Play_state)) , !.
set_local_next_play_state(Play_state) :- retract(play_state('local_next_play_state',_)), assert(play_state('local_next_play_state',Play_state)).
get_local_next_play_state(Play_state) :- play_state('local_next_play_state',Play_state).

determine_sides(Play_state) :- (Play_state =:= 7 ; Play_state =:= 13) , team_left(Team) , set_left(Team) , find_goalies.
team_left(Team) :- roboter(Team,_,Pos_x,_,_,_,1) , Pos_x < 0 .
set_left(Team) :- not(left_team('left_team',_)) , assert(left_team('left_team',Team)) , !.
set_left(Team) :- retract(left_team('left_team',_)) , assert(left_team('left_team',Team)).
get_left(Team) :- left_team('left_team',Team).
find_goalies :- find_left_goalie , find_right_goalie.
find_left_goalie :- field('field',Field_width,_,_,_,_,_,_,_) , left_team('left_team',Left_team) , roboter(Left_team,Id1,Pos_x1,Pos_y1,_,_,1) , distance(Pos_x1,Pos_y1,-(Field_width/2),0,Dist1) , not((roboter(Left_team,Id2,Pos_x2,Pos_y2,_,_,1) , Id1 =\= Id2 , distance(Pos_x2,Pos_y2,-(Field_width/2),0,Dist2) , Dist2 < Dist1)) , set_left_goalie(Left_team,Id1).
find_right_goalie :- field('field',Field_width,_,_,_,_,_,_,_) , left_team('left_team',Left_team) , roboter(Team,Id1,Pos_x1,Pos_y1,_,_,1) , Team =\= Left_team , distance(Pos_x1,Pos_y1,(Field_width/2),0,Dist1) , not((roboter(Team,Id2,Pos_x2,Pos_y2,_,_,1) , Id1 =\= Id2 , distance(Pos_x2,Pos_y2,(Field_width/2),0,Dist2) , Dist2 < Dist1)) , set_right_goalie(Team,Id1).
set_left_goalie(Team, Id) :- not(goalie('left_goalie',_,_)) , assert(goalie('left_goalie',Team,Id)) , !.
set_left_goalie(Team, Id) :- retract(goalie('left_goalie',_,_)) , assert(goalie('left_goalie',Team,Id)).
set_right_goalie(Team, Id) :- not(goalie('right_goalie',_,_)) , assert(goalie('right_goalie',Team,Id)) , !.
set_right_goalie(Team, Id) :- retract(goalie('right_goalie',_,_)) , assert(goalie('right_goalie',Team,Id)).

%testing and helping
distance(X1,Y1,X2,Y2,Dist) :- Dist is sqrt((X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1)).
distance_to_left_def_area(X1,Y1,Dist) :- field('field',Field_width,_,_,_,_,Defense_radius,Defense_line,_) ,
                                         ( (Y1 =< -(Defense_line/2) , distance(X1,Y1,-(Field_width/2),-(Defense_line/2),Dist_tmp)) ;
                                           (Y1 =< (Defense_line/2) , Y1 >= -(Defense_line/2) , Dist_tmp is abs(X1+(Field_width/2))) ;
                                           (Y1 >= (Defense_line/2) , distance(X1,Y1,-(Field_width/2),(Defense_line/2),Dist_tmp) ) ) ,
                                          Dist is (Dist_tmp - Defense_radius) , !.
distance_to_right_def_area(X1,Y1,Dist) :- field('field',Field_width,_,_,_,_,Defense_radius,Defense_line,_) ,
                                          ( (Y1 =< -(Defense_line/2) , distance(X1,Y1,(Field_width/2),-(Defense_line/2),Dist_tmp)) ;
                                            (Y1 =< (Defense_line/2) , Y1 >= -(Defense_line/2) , Dist_tmp is abs(X1-(Field_width/2))) ;
                                            (Y1 >= (Defense_line/2) , distance(X1,Y1,(Field_width/2),(Defense_line/2),Dist_tmp) ) ) ,
                                          Dist is (Dist_tmp - Defense_radius) , !.



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%															%%
%%		Rules and Laws												%%
%%															%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel0: Nur Regeln checken falls interner und externer Status gleich oder interner Status == running			%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_zero :- get_global_play_state(X) , (get_local_play_state(X) ; get_local_play_state(2)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel30: Ball im Spielfeld, nur im running										%%
%% coresspond to law 9: The Ball In and Out of Play									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ball_in_game(Field_width,Field_height,Pos_x,Pos_y) :- Pos_x > -(Field_width/2) , Pos_x < Field_width/2 , Pos_y > -(Field_height/2) , Pos_y < Field_height/2.

rule_thirty_before :- get_local_play_state(2).

rule_thirty :- rule_thirty_before , field('field',Field_width,Field_height,_,_,_,_,_,_) , ball_location('ball',Pos_x,Pos_y,_,_,_,_) , not(ball_in_game(Field_width,Field_height,Pos_x,Pos_y)) , rule_thirty_after.

rule_thirty_after :- (set_freekick_pos_throw_in ; set_freekick_pos_goal_out) , ball_status('ball',Ltt,Ltid,_,_) , set_rule_breaker(Ltt,Ltid).
set_freekick_pos_throw_in :- ball_location('ball',Pos_x,Pos_y,_,_,_,_) , ball_status('ball',Last_touched_team,_,_,_) , (throw_in_above(Pos_x,Pos_y) ; throw_in_below(Pos_x,Pos_y)) , Next is (12-(Last_touched_team*6)) , set_next_play_state(Next).
throw_in_above(Pos_x,Pos_y) :- field('field',_,Field_height,_,_,_,_,_,_) , Pos_y > Field_height/2 , Pos_y_new is (Field_height/2)-100 , set_freekick_pos(Pos_x,Pos_y_new,0).
throw_in_below(Pos_x,Pos_y) :- field('field',_,Field_height,_,_,_,_,_,_) , Pos_y < -(Field_height/2) , Pos_y_new is -((Field_height/2)-100) , set_freekick_pos(Pos_x,Pos_y_new,0).
set_freekick_pos_goal_out :- ball_location('ball',Pos_x,Pos_y,_,_,_,_) , ball_status('ball',Last_touched_team,_,_,_) , (goal_out_left(Pos_x,Pos_y,Last_touched_team) ; goal_out_right(Pos_x,Pos_y,Last_touched_team)) , Next is (11-(Last_touched_team*6)) , set_next_play_state(Next).
%corner left above
goal_out_left(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x < -(Field_width/2) , (Last_touched_team =:= Left_team) , Pos_y > 0 , Pos_x_new is -(Field_width/2)+100, Pos_y_new is (Field_height/2)-100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%corner left below
goal_out_left(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x < -(Field_width/2) , (Last_touched_team =:= Left_team) , Pos_y < 0 , Pos_x_new is -(Field_width/2)+100, Pos_y_new is -(Field_height/2)+100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%goal kick left above
goal_out_left(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x < -(Field_width/2) , (Last_touched_team =\= Left_team) , Pos_y > 0 , Pos_x_new is -(Field_width/2)+500, Pos_y_new is (Field_height/2)-100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%goal kick left below
goal_out_left(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x < -(Field_width/2) , (Last_touched_team =\= Left_team) , Pos_y < 0 , Pos_x_new is -(Field_width/2)+500, Pos_y_new is -(Field_height/2)+100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%corner right above
goal_out_right(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x > (Field_width/2) , (Last_touched_team =\= Left_team) , Pos_y > 0 , Pos_x_new is (Field_width/2)-100, Pos_y_new is (Field_height/2)-100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%corner kick right below
goal_out_right(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x > (Field_width/2) , (Last_touched_team =\= Left_team) , Pos_y < 0 , Pos_x_new is (Field_width/2)-100, Pos_y_new is -(Field_height/2)+100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%goal kick right above
goal_out_right(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x > (Field_width/2) , (Last_touched_team =:= Left_team) , Pos_y > 0 , Pos_x_new is (Field_width/2)-500, Pos_y_new is (Field_height/2)-100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).
%goal kick right below
goal_out_right(Pos_x,Pos_y,Last_touched_team) :- field('field',Field_width,Field_height,_,_,_,_,_,_) , left_team('left_team',Left_team) , Pos_x > (Field_width/2) , (Last_touched_team =:= Left_team) , Pos_y < 0 , Pos_x_new is (Field_width/2)-500, Pos_y_new is -(Field_height/2)+100 , set_freekick_pos(Pos_x_new,Pos_y_new,0).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel16: Roboterposition im Stopped											%%
%% coresspond to law ?: 												%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_sixteen :- get_local_play_state(1) , roboter(Team,ID,Pos_x_bot,Pos_y_bot,_,_,1) , ball_location('ball',Pos_x_ball,Pos_y_ball,_,_,_,_) , distance(Pos_x_bot,Pos_y_bot,Pos_x_ball,Pos_y_ball,Dist) , Dist < 500 , set_rule_breaker(Team,ID).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel15: Roboterabstand bei Freekick											%%
%% coresspond to law 13: Free Kicks											%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_fifteen_before :- freekick_blue ; freekick_yellow.
freekick_yellow :- get_local_play_state(3).
freekick_yellow :- get_local_play_state(5).
freekick_yellow :- get_local_play_state(6).
freekick_yellow :- get_local_play_state(7).
freekick_blue :- get_local_play_state(9).
freekick_blue :- get_local_play_state(11).
freekick_blue :- get_local_play_state(12).
freekick_blue :- get_local_play_state(13).
rule_fifteen :- rule_fifteen_before , rule_fifteen_distance_to_ball.
rule_fifteen :- rule_fifteen_before , rule_fifteen_distance_to_opp_defence_area.
rule_fifteen_distance_to_ball :- roboter(Team,ID,Pos_x_bot,Pos_y_bot,_,_,1) , Team =:= 0 , freekick_blue , ball_location('ball',Pos_x_ball,Pos_y_ball,_,_,_,_) , distance(Pos_x_bot,Pos_y_bot,Pos_x_ball,Pos_y_ball,Dist) , Dist < 500 , set_rule_breaker(Team,ID).
rule_fifteen_distance_to_ball :- roboter(Team,ID,Pos_x_bot,Pos_y_bot,_,_,1) , Team =:= 1 , freekick_yellow , ball_location('ball',Pos_x_ball,Pos_y_ball,_,_,_,_) , distance(Pos_x_bot,Pos_y_bot,Pos_x_ball,Pos_y_ball,Dist) , Dist < 500 , set_rule_breaker(Team,ID).
rule_fifteen_distance_to_opp_defence_area :- roboter(Team,ID,Pos_x,Pos_y,_,_,1) ,
                                           get_left(Left_team) ,
                                           ( (Left_team =:= Team , distance_to_right_def_area(Pos_x,Pos_y,Dist)) ;
                                             (Left_team =\= Team , distance_to_left_def_area(Pos_x,Pos_y,Dist)) ) ,
                                           Dist =< 200 ,
                                           set_rule_breaker(Team,ID).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel14: Roboterpositionen bei Kickoff										%%
%% coresspond to law 8: The Start and Restart of Play									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_fourteen_before :- kickoff_blue ; kickoff_yellow.
kickoff_blue :- get_local_play_state(X) , (X =:= 9 ; X =:= 13).
kickoff_yellow :- get_local_play_state(X) , (X =:= 3 ; X =:= 7).
rule_fourteen :- rule_fourteen_before , get_left(Left_team) , roboter(Team,ID,Pos_x,_,_,_,1) , ( (Left_team =:= Team , Pos_x > 0) ; (Left_team =\= Team , Pos_x < 0) ) , set_rule_breaker(Team,ID).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel3: Torwartwechsel												%%
%% coresspond to law 3: The Number of Robots										%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_three :- (check_left_goalie ; check_right_goalie).
check_left_goalie :- field('field',Field_width,_,_,_,_,_,_,_) , get_left(Left_team) , goalie('left_goalie',Left_team,Id1) , roboter(Left_team,Id1,Pos_x1,Pos_y1,_,_,1) , distance(Pos_x1,Pos_y1,-(Field_width/2),0,Dist1) , roboter(Left_team,Id2,Pos_x2,Pos_y2,_,_,1) , Id1 =\= Id2 , distance(Pos_x2,Pos_y2,-(Field_width/2),0,Dist2) , Dist2 < Dist1 , set_rule_breaker(Left_team,Id2).
check_right_goalie :- field('field',Field_width,_,_,_,_,_,_,_) , get_left(Left_team) , goalie('right_goalie',Team,Id1) , Team =\= Left_team , roboter(Team,Id1,Pos_x1,Pos_y1,_,_,1) , distance(Pos_x1,Pos_y1,(Field_width/2),0,Dist1) , roboter(Team,Id2,Pos_x2,Pos_y2,_,_,1) , Id1 =\= Id2 , distance(Pos_x2,Pos_y2,(Field_width/2),0,Dist2) , Dist2 < Dist1 , set_rule_breaker(Team,Id2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel29: Ball im Tor													%%
%% coresspond to law ?: 												%%
%% nur im running, nur wenn oft genug berührt und nur wenn nicht Flying (Also auch Regel 13 erfüllt)			%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ball_in_right_goal :- field('field',Field_width,_,Goal_width,Goal_Depth,Goal_Height,_,_,_) , ball_location('ball',Pos_x,Pos_y,Pos_z,_,_,_) , Pos_x > (Field_width/2) , Pos_x < ((Field_width/2)+Goal_Depth) , Pos_y > -(Goal_width/2) , Pos_y < (Goal_width/2) , Pos_z < Goal_Height.
ball_in_left_goal :- field('field',Field_width,_,Goal_width,Goal_Depth,Goal_Height,_,_,_) , ball_location('ball',Pos_x,Pos_y,Pos_z,_,_,_) , Pos_x < -(Field_width/2) , Pos_x > -((Field_width/2)+Goal_Depth) , Pos_y > -(Goal_width/2) , Pos_y < (Goal_width/2) , Pos_z < Goal_Height.
second_touch :- ball_status('ball',Ltt,Ltid,Status,X) , Status =\= 3 , X > 0 , set_rule_breaker(Ltt,Ltid).

rule_twentynine_before :- get_local_play_state(2).
rule_twentynine :- rule_twentynine_before , second_touch , ((ball_in_right_goal , rule_twentynine_after(1)) ; (ball_in_left_goal , rule_twentynine_after(0))).
rule_twentynine_after(Side) :- get_left(Left_team) , ( (Side =:= 1 , goal(Left_team) , Next is (13-(Left_team*6)),  set_next_play_state(Next)) ; ( Side =:= 0,  goal(abs(Left_team-1)) , Next is (13-(abs(Left_team-1)*6)) , set_next_play_state(Next) ) ) , set_freekick_pos(0,0,0).

set_goals(X,Y) :- retract(result('result',_,_)) , assert(result('result',X,Y)).
goal(X) :- get_standing(Y,B) , Z is Y+abs(X-1) , C is B+X , set_goals(Z,C).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel17: Roboterpositionen beim Penalty										%%
%% coresspond to law 14: The Penalty Kick										%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
penalty_yellow :- get_local_play_state(X) , (X =:= 4 ; X =:= 8).
penalty_blue :- get_local_play_state(X) , (X =:= 10 ; X =:= 14).
rule_seventeen :- rule_seventeen_positions.

rule_seventeen_positions :- get_left(0) , penalty_blue , penalty_blue_on_left_goal.
rule_seventeen_positions :- get_left(1) , penalty_yellow , penalty_yellow_on_left_goal.
rule_seventeen_positions :- get_left(1) , penalty_blue , penalty_blue_on_right_goal.
rule_seventeen_positions :- get_left(0) , penalty_yellow , penalty_yellow_on_right_goal.

%Blau aufs linke Tor
penalty_blue_on_left_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is ((-(Field_width))+Penalty_mark+400) , goalie('left_goalie',0,Goalie_id) , roboter(0,Other_id,Pos_x_other,_,_,_,1) , Goalie_id =\= Other_id , Pos_x_other < Min_dist , set_rule_breaker(0,Other_id).
penalty_blue_on_left_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is ((-(Field_width))+Penalty_mark+400) , not((roboter(1,_,Pos_x_kicker,_,_,_,1) , Pos_x_kicker < Min_dist)) , set_rule_breaker(1,12).
penalty_blue_on_left_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is ((-(Field_width))+Penalty_mark+400) , roboter(1,Kicker_id,Pos_x_kicker,_,_,_,1) , roboter(1,Other_id,Pos_x_other,_,_,_,1) , Kicker_id =\= Other_id , Pos_x_kicker < Min_dist , Pos_x_other < Min_dist , ( ( Pos_x_kicker < Pos_x_other , set_rule_breaker(1,Other_id)) ; ( Pos_x_kicker > Pos_x_other , set_rule_breaker(1,Kicker_id)) ).

%Gelb aufs linke Tor
penalty_yellow_on_left_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is ((-(Field_width))+Penalty_mark+400) , goalie('left_goalie',1,Goalie_id) , roboter(1,Other_id,Pos_x_other,_,_,_,1) , Goalie_id =\= Other_id , Pos_x_other < Min_dist , set_rule_breaker(1,Other_id).
penalty_yellow_on_left_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is ((-(Field_width))+Penalty_mark+400) , not((roboter(0,_,Pos_x_kicker,_,_,_,1) , Pos_x_kicker < Min_dist)) , set_rule_breaker(0,12).
penalty_yellow_on_left_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is ((-(Field_width))+Penalty_mark+400) , roboter(0,Kicker_id,Pos_x_kicker,_,_,_,1) , roboter(0,Other_id,Pos_x_other,_,_,_,1) , Kicker_id =\= Other_id , Pos_x_kicker < Min_dist , Pos_x_other < Min_dist , ( ( Pos_x_kicker < Pos_x_other , set_rule_breaker(1,Other_id)) ; ( Pos_x_kicker > Pos_x_other , set_rule_breaker(0,Kicker_id)) ).

%Blau aufs rechte Tor
penalty_blue_on_right_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is (Field_width-Penalty_mark-400) , goalie('right_goalie',0,Goalie_id) , roboter(0,Other_id,Pos_x_other,_,_,_,1) , Goalie_id =\= Other_id , Pos_x_other > Min_dist , set_rule_breaker(0,Other_id).
penalty_blue_on_right_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is (Field_width-Penalty_mark-400) , not((roboter(1,_,Pos_x_kicker,_,_,_,1) , Pos_x_kicker > Min_dist)) , set_rule_breaker(1,12).
penalty_blue_on_right_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is (Field_width-Penalty_mark-400) , roboter(1,Kicker_id,Pos_x_kicker,_,_,_,1) , roboter(1,Other_id,Pos_x_other,_,_,_,1) , Kicker_id =\= Other_id , Pos_x_kicker > Min_dist , Pos_x_other > Min_dist , ( ( Pos_x_kicker > Pos_x_other , set_rule_breaker(1,Other_id)) ; ( Pos_x_kicker < Pos_x_other , set_rule_breaker(1,Kicker_id)) ).

%Gelb aufs rechte Tor
penalty_yellow_on_right_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is (Field_width-Penalty_mark-400) , goalie('right_goalie',1,Goalie_id) , roboter(1,Other_id,Pos_x_other,_,_,_,1) , Goalie_id =\= Other_id , Pos_x_other > Min_dist , set_rule_breaker(1,Other_id).
penalty_yellow_on_right_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is (Field_width-Penalty_mark-400) , not((roboter(0,_,Pos_x_kicker,_,_,_,1) , Pos_x_kicker > Min_dist)) , set_rule_breaker(0,12).
penalty_yellow_on_right_goal :- field('field',Field_width,_,_,_,_,_,_,Penalty_mark) , Min_dist is (Field_width-Penalty_mark-400) , roboter(0,Kicker_id,Pos_x_kicker,_,_,_,1) , roboter(0,Other_id,Pos_x_other,_,_,_,1) , Kicker_id =\= Other_id , Pos_x_kicker > Min_dist , Pos_x_other > Min_dist , ( ( Pos_x_kicker > Pos_x_other , set_rule_breaker(1,Other_id)) ; ( Pos_x_kicker < Pos_x_other , set_rule_breaker(0,Kicker_id)) ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel28: Timeout-Zeit überschritten											%%
%% coresspond to law 28: The Duration of the Match									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_twentyeight :- get_timeout_start_yellow(Start) , Start =\= 0 , get_timestamp(Timestamp) , get_timeout_time_yellow(Old) , New is Old+(Timestamp-Start) , New > 300000.
rule_twentyeight :- get_timeout_start_blue(Start) , Start =\= 0 , get_timestamp(Timestamp) , get_timeout_time_blue(Old) , New is Old+(Timestamp-Start) , New > 300000.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel27: Anzahl Timeouts												%%
%% coresspond to law 28: The Duration of the Match									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_twentyseven :- get_timeout_start_yellow(Start) , Start =\= 0 , timeouts('timeouts',Yellow,_) , Yellow > 4.
rule_twentyseven :- get_timeout_start_blue(Start) , Start =\= 0 , timeouts('timeouts',_,Blue) , Blue > 4.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel24: Gesamt-Zeit													%%
%% coresspond to law 28: The Duration of the Match									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_twentyfour :- get_total_play_time_start(Start) , Start =\= 0 , get_total_play_time_total(Total) , get_total_play_time_start(Start) , get_timestamp(Now) , Sum is Total+(Now-Start) , Sum > 600000.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel18: Nicht-Torhüter im Strafraum											%%
%% coresspond to law ?: 												%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_eighteen :- get_local_play_state(2) , goalie('left_goalie',Team,Id1) , roboter(Team,Id2,Pos_x2,Pos_y2,_,_,1) , Id1 =\= Id2 , distance_to_left_def_area(Pos_x2,Pos_y2,Dist) , Dist < 0 , set_rule_breaker(Team,Id2).
rule_eighteen :- get_local_play_state(2) , goalie('right_goalie',Team,Id1) , roboter(Team,Id2,Pos_x2,Pos_y2,_,_,1) , Id1 =\= Id2 , distance_to_right_def_area(Pos_x2,Pos_y2,Dist) , Dist < 0 , set_rule_breaker(Team,Id2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel23: 10 Sekunden Begrenzung bei Freistößen									%%
%% coresspond to law 9: The Ball In and Out of Play									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_twentythree :- get_local_play_state(PS) , ( (PS =:= 3) ; (PS =:= 4) ; (PS =:= 5) ; (PS =:= 6) ; (PS =:= 9) ; (PS =:= 10) ; (PS =:= 11) ; (PS =:= 12) ) , get_ten_seconds(Ten) , Ten =\= 0 , get_timestamp(Time) , Diff is Time-Ten , Diff > 10000 , set_local_play_state(1) , set_local_next_play_state(2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel19: Torwartberührung												%%
%% coresspond to law 12: Fouls and Misconduct										%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_nineteen :- get_local_play_state(2) , goalie('left_goalie',Team1,Id1) , roboter(Team1,Id1,Pos_x1,Pos_y1,_,_,1) , roboter(Team2,Id2,Pos_x2,Pos_y2,_,_,1) , Team1 =\= Team2 , distance_to_left_def_area(Pos_x2,Pos_y2,Dist1) , Dist1 < 0 , distance(Pos_x1,Pos_y1,Pos_x2,Pos_y2,Dist2) , Dist2 < 200 , set_rule_breaker(Team2,Id2) , Freekick_x is (Pos_x2 + ((Pos_x1-Pos_x2)/2)) , Freekick_y is (Pos_y2 + ((Pos_y1-Pos_y2)/2)) , set_freekick_pos(Freekick_x,Freekick_y,0) , New is 5+(Team1*6) , set_next_play_state(New).
rule_nineteen :- get_local_play_state(2) , goalie('right_goalie',Team1,Id1) , roboter(Team1,Id1,Pos_x1,Pos_y1,_,_,1) , roboter(Team2,Id2,Pos_x2,Pos_y2,_,_,1) , Team1 =\= Team2 , distance_to_right_def_area(Pos_x2,Pos_y2,Dist1) , Dist1 < 0 , distance(Pos_x1,Pos_y1,Pos_x2,Pos_y2,Dist2) , Dist2 < 200 , set_rule_breaker(Team2,Id2) , Freekick_x is (Pos_x2 + ((Pos_x1-Pos_x2)/2)) , Freekick_y is (Pos_y2 + ((Pos_y1-Pos_y2)/2)) , set_freekick_pos(Freekick_x,Freekick_y,0) , New is 5+(Team1*6) , set_next_play_state(New).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel22: Freistossausführung im before_										%%
%% coresspond to law 9: The Ball In and Out of Play									%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_twentytwo :- get_local_play_state(PS) , ( (PS =:= 7) ; (PS =:= 8) ; (PS =:= 13) ; (PS =:= 14) ) , ball_status('ball',Ltt,Ltid,_,Touch) , Touch > 0 , set_local_play_state(1) , set_local_next_play_state(PS), set_rule_breaker(Ltt,Ltid).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel1: Es sind nur max 6 Roboter pro Team erlaubt									%%
%% coresspond to law 3: The Number of Robots										%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_one :- rule_one(0) ; rule_one(1).
rule_one(Team) :- roboter(Team,ID1,_,_,_,_,1) , 
                  roboter(Team,ID2,_,_,_,_,1) , ID1 =\= ID2 ,
                  roboter(Team,ID3,_,_,_,_,1) , ID1 =\= ID3 , ID2 =\= ID3 ,
                  roboter(Team,ID4,_,_,_,_,1) , ID1 =\= ID4 , ID2 =\= ID4 , ID3 =\= ID4 ,
                  roboter(Team,ID5,_,_,_,_,1) , ID1 =\= ID5 , ID2 =\= ID5 , ID3 =\= ID5 , ID4 =\= ID5 ,
                  roboter(Team,ID6,_,_,_,_,1) , ID1 =\= ID6 , ID2 =\= ID6 , ID3 =\= ID6 , ID4 =\= ID6 , ID5 =\= ID6 ,
                  roboter(Team,ID7,_,_,_,_,1) , ID1 =\= ID7 , ID2 =\= ID7 , ID3 =\= ID7 , ID4 =\= ID7 , ID5 =\= ID7 , ID6 =\= ID7 ,
                  set_rule_breaker(Team,ID6).
                  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Regel42: Abseits													%%
%% coresspond to no law, in law 11 (Offside) it is declared, that there is no offside					%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rule_fourtytwo :- check_offside(1) , retract(check_offside(_)) , assert(check_offside(0)) , rule_fourtytwo_offside.
%Left Team
rule_fourtytwo_offside :- ball_status('ball',Ltt,Ltid,_,_) , get_left(Ltt) , roboter(Ltt,Ltid,Pos_xt,_,_,_,1) , roboter(Ltt,ID1,Pos_x1,Pos_y1,_,_,1) , Ltid =\= ID1 , Pos_x1 > Pos_xt , Pos_x1 > 0 , not(( roboter(Team,ID2,Pos_x2,_,_,_,1) , Team =\= Ltt , roboter(Team,ID3,Pos_x3,_,_,_,1) , ID2 =\= ID3 , Pos_x2 > Pos_x1 , Pos_x3 > Pos_x1 )) , set_rule_breaker(Ltt,ID1) , set_freekick_pos(Pos_x1,Pos_y1,0).
%Right Team
rule_fourtytwo_offside :- ball_status('ball',Ltt,Ltid,_,_) , get_left(Team) , Team =\= Ltt , roboter(Ltt,Ltid,Pos_xt,_,_,_,1) , roboter(Ltt,ID1,Pos_x1,Pos_y1,_,_,1) , Ltid =\= ID1 , Pos_x1 < Pos_xt , Pos_x1 < 0 , not(( roboter(Team,ID2,Pos_x2,_,_,_,1) , roboter(Team,ID3,Pos_x3,_,_,_,1) , ID2 =\= ID3 , Pos_x2 < Pos_x1 , Pos_x3 < Pos_x1 )) , set_rule_breaker(Ltt,ID1) , set_freekick_pos(Pos_x1,Pos_y1,0).
