RHCR
```
./build/unified_runner \
  --planner RHCR \
  --map extern/RHCR/maps/32x32_allEntry.grid \
  --input_task_csv task/tasks_group800_blank_time_blank.csv \
  --num_agents 50 \
  --batch_size 10
```
LaCAM
```
./build/unified_runner \
  --planner LaCAM \
  --map map/32x32_allEntry.grid \
  --input_task_csv task/tasks_group800_blank_time_blank.csv \
  --num_agents 100 \
  --batch_size 50
```
  


