import random

# Define the operations and keys
operations = ['R', 'W', 'I']
keys = ['key1', 'key2', 'key3', 'key4', 'key5']

# Generate approximately 10,000 operations
num_operations = 10000

# Open the file for writing
with open('large.txt', 'w') as file:
    for _ in range(num_operations):
        # Choose a random operation and key
        op = random.choice(operations)
        key = random.choice(keys)
        
        # Generate a line with the chosen operation and key
        if op == 'R' or op == 'D':
            line = f"{op},{key}\n"
        else:
            # For write and increase operations, generate a random value
            value = random.randint(1, 100)
            line = f"{op},{key},{value}\n"
        
        # Write the line to the file
        file.write(line)

