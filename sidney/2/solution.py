symbol_score_map_1 = {
    'X': 1, # Rock
    'Y': 2, # Paper
    'Z': 3, # Scissors
}

symbol_score_map_2 = {
    'A': 1, # Rock
    'B': 2, # Paper
    'C': 3, # Scissors
}

symbol_win_score_map = {
    'X': 0,
    'Y': 3,
    'Z': 6,
}

win_score_map = {
    'AX': 3,
    'AY': 6,
    'AZ': 0,
    'BX': 0,
    'BY': 3,
    'BZ': 6,
    'CX': 6,
    'CY': 0,
    'CZ': 3
}

strategy_map = {
    'AX': 'C',
    'AY': 'A',
    'AZ': 'B',
    'BX': 'A',
    'BY': 'B',
    'BZ': 'C',
    'CX': 'B',
    'CY': 'C',
    'CZ': 'A',
}

file1 = open('./sidney/2/data.txt', 'r')
lines = file1.readlines()

score = 0
for line in lines:
    other = line[0]
    own = line[2]
    score = score + symbol_score_map_1[own] + win_score_map[f'{other}{own}']

print(f'Score part 1 is {score}') 

score = 0
for line in lines:
    strategy_code = f'{line[0]}{line[2]}'
    symbol = strategy_map[strategy_code]
    game_result = line[2]
    score = score + symbol_win_score_map[game_result] + symbol_score_map_2[symbol]

print(f'Score part 2 is {score}') 
