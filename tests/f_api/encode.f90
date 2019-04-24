
module fapi_encode_tests

    use odc
    use odc_config
    use, intrinsic :: iso_fortran_env
    implicit none

    integer(8), parameter :: col1_data(7) = [1, 2, 3, 4, 5, 6, 7]
    integer(8), parameter :: col2_data(7) = [0, 0, 0, 0, 0, 0, 0]
    integer(8), parameter :: col3_data(7) = [73, 73, 73, 73, 73, 73, 73]
    real(8), parameter :: col4_data(7) = [1.432, 1.432, 1.432, 1.432, 1.432, 1.432, 1.432]
    integer(8), parameter :: col5_data(7) = [-17, -7, -7, 999999, 1, 4, 4]
    character(16), parameter :: col6_data(7) = [character(16) :: "aoeu", "aoeu", "abcdefghijkl", "None", "boo", "squiggle", "a"]
    character(8), parameter :: col7_data(7) = [character(8) :: "abcd", "abcd", "abcd", "abcd", "abcd", "abcd", "abcd"]
    real(8), parameter :: col8_data(7) = [2.345, 2.345, 2.345, 2.345, 2.345, 2.345, 2.345]
    real(8), parameter :: col9_data(7) = [999.99, 888.88, 777.77, 666.66, 999999.0, 444.44, 333.33]
    real(8), parameter :: col10_data(7) = [999.99, 888.88, 777.77, 666.66, 999999.0, 444.44, 333.33]
    integer(8), parameter :: col11_data(7) = [1, 999999, 3, 4, 5, 999999, 8]
    integer(8), parameter :: col12_data(7) = [-512, 999999, 3, 7623, -22000, 999999, 7]
    integer(8), parameter :: col13_data(7) = [-1234567, 8765432, 999999, 22, 2222222, -81222323, 999999]
    integer(8), parameter :: col14_data(7) = [999999, 999999, 999999, 999999, 999999, 999999, 999999]

contains

    ! TODO: Test missing doubles
    ! TODO: Test missing REAL
    ! TODO: Encoding NaN?

    function test_encode_column_major() result(success)

        real(8) :: data(7, 15)
        integer :: row, outunit
        type(odc_encoder) :: encoder
        logical :: success
        success = .true.

        ! Fill in an array of data to encode

        data(:, 1) = col1_data
        data(:, 2) = col2_data
        data(:, 3 ) = col4_data
        data(:, 4) = col4_data
        data(:, 5) = col5_data
        do row = 1, 7
            data(row, 6:7) = transfer(col6_data(row), 1.0_8, 2)
            data(row, 8) = transfer(col7_data(row), 1.0_8)
        end do
        data(:, 9) = col8_data
        data(:, 10) = col9_data
        data(:, 11) = col10_data
        data(:, 12) = col11_data
        data(:, 13) = col12_data
        data(:, 14) = col13_data
        data(:, 15) = col14_data

        call encoder%initialise()

        if (encoder%add_column("col1", ODC_INTEGER)    /= 1 .or. &
            encoder%add_column("col2", ODC_INTEGER)    /= 2 .or. &
            encoder%add_column("col3", ODC_BITFIELD)   /= 3 .or. &
            encoder%add_column("col4", ODC_DOUBLE)     /= 4 .or. &
            encoder%add_column("col5", ODC_INTEGER)    /= 5 .or. &
            encoder%add_column("col6", ODC_STRING)     /= 6 .or. &
            encoder%add_column("col7", ODC_STRING)     /= 7 .or. &
            encoder%add_column("col8", ODC_REAL)       /= 8 .or. &
            encoder%add_column("col9", ODC_DOUBLE)     /= 9 .or. &
            encoder%add_column("col10", ODC_REAL)      /= 10 .or. &
            encoder%add_column("col11", ODC_BITFIELD)  /= 11 .or. &
            encoder%add_column("col12", ODC_INTEGER)   /= 12 .or. &
            encoder%add_column("col13", ODC_INTEGER)   /= 13 .or. &
            encoder%add_column("col14", ODC_INTEGER)   /= 14) then
            write(error_unit, *) 'Unexpected column number after adding column'
            success = .false.
        end if

        call encoder%column_set_size_doubles(6, 2)
        call encoder%set_data_array(data)

        open(newunit=outunit, file='testout.odb', access='stream', form='unformatted')
        write(*,*) encoder%encode(outunit)

        call encoder%free()

    end function

    !funcion test_encode_integers() result(success)
    !    logical :: success
    !    success = .true.
    !end function

    !function test_encode_columns() result(success)
    !    logical :: success
    !    success = .true.
    !end function

end module


program fapi_general

    use fapi_encode_tests
    implicit none

    logical :: success

    call odc_initialise_api()
    call odc_set_missing_integer(999999_8)
    call odc_set_missing_double(999999.0_8)

    success = .true.
    success = test_encode_mixed() .and. success
    !success = test_encode_integers() .and. success
    !success = test_encode_columns() .and. success

    if (.not. success) stop -1

end program
