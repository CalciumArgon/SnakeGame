CC := gcc
CXX := g++

.PHONY : task
# 所有需要链接的 Object 文件
objects := main.o field.o item.o snake.o clock.o game.o

# 链接
task: $(objects)
	$(CXX) -o $@ $(objects)

main.o: field.h
field.o: snake.h
item.o: field.h snake.h
snake.o: item.h
game.o: snake.h clock.h


.PHONY : clean clean_makefile run
clean:
	@rm -f task $(objects)
	@echo "======= Delete all the Object-File ======="
	@echo "======= Delete the Executable-File ======="

clean_makefile:
	@rm -f $(objects)
	@echo "======= Delete all the Object-File ======="

# 运行生成的 task 可执行文件
run:
	@./task