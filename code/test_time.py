import time

prev_time = time.time() * 1000
curr_time = time.time() * 1000
elapsed_time= 0

while True:
    curr_time = time.time() * 1000
    elapsed_time = curr_time - prev_time
    print("elabsed time ", elapsed_time)

    #do stff here
     
    prev_time = curr_time
    # Add a delay or perform other tasks
    time.sleep(0.1)  # Sleep for 0.1 seconds as an example