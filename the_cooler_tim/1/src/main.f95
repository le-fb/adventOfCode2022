program aoc_1 

    implicit none

    integer :: stat                                     ! file status
    character(100) :: io_msg                            
    integer :: calories, temp, out                      
    integer, dimension(3) :: top_three, temp_arr        ! contains the top three calorie elfs
    logical, dimension(3) :: comp

    calories = 0
    temp = 0
    top_three = [0,0,0]
    temp_arr = [0,0,0]

    ! File Read
    open(11, file='../src/data/calories.dat', iostat=stat, iomsg=io_msg)
    if(stat == 0) then
        do   
          if(stat /= 0) exit                            ! stat will be !=0 at EOF
            read(11, '(I6)', iostat = stat) out
            temp = temp + out
            if(out == 0) then                           ! empty line will be parsed as "0" with the 'I6' format
                temp_arr ( : ) = temp                   ! each element of the array has the current calorie-sum as its value
                comp = ( temp_arr >= top_three)         ! compare
                if(comp(1)) then 
                    top_three(3) = top_three(2)
                    top_three(2) = top_three(1)
                    top_three(1) = temp_arr(1)
                else if(comp(2)) then 
                    top_three(3) = top_three(2)
                    top_three(2) = temp_arr(2)
                else if(comp(3)) then 
                    top_three(3) = temp_arr(3) 
                end if 
                temp = 0
            end if
        end do
    else
        write(*,*) 'Fix your stupid file, what the hell is this!?'
        write(*,*) 'IO_STAT: ', stat
        write(*,*) io_msg
    end if
    close(11)
    write(*,*) 'Found the top three elves with the most impressive bulking diets:'
    write(*,*) top_three(1), top_three(2), top_three(3)
    write(*,*) 'In total they have ',sum(top_three), ' calories!'
end program aoc_1