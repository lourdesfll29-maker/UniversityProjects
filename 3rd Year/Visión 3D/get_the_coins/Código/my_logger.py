"""
This file develops generic logger class.
"""

__author__ = "Marcos Belda Martinez"
__copyright__ = "Copyright 2025, 3D Vision"
__license__ = "GPL"
__version__ = "1.0"
__maintainer__ = "Marcos Belda Martinez"
__email__ = "mbelmar@etsinf.upv.es"
__status__ = "Development"


# ---------------------------------------------------------------------------- #
# NEEDED LIBRARIES

### External Libraries
import inspect
import datetime

### Own Libraries


# ---------------------------------------------------------------------------- #
# LOGGER CLASS

class MyLogger:
    """
    A customizable logger class for handling different logging levels and
    formats.

    :cvar logging_levels: List of supported logging levels.
    :type logging_levels: list[str]
    :cvar logging_colors: Dictionary mapping logging levels to their respective
                          colors.
    :type logging_colors: dict[logging_levels, str]
    :ivar level: The current logging level of the logger instance.
    :type level: logging_levels
    :ivar show_timestamp: Flag indicating whether to display timestamps in log
                          messages.
    :type show_timestamp: bool
    :ivar show_caller_function: Flag indicating whether to display the caller
                                function name in log messages.
    :type show_caller_function: bool
    """

    logging_levels: list[str] = [
        'DEBUG',
        'INFO',
        'WARNING',
        'ERROR',
        'CRITICAL'
    ]

    logging_colors: dict[logging_levels, str] = {
        'DEBUG': '\033[92m',  # Define green color for DEBUG outputs
        'INFO': '\033[0m',  # Define default color for INFO outputs
        'WARNING': '\033[93m',  # Define yellow color for WARNING outputs
        'ERROR': '\033[91m',  # Define red color for ERROR outputs
        'CRITICAL': '\033[95m',  # Define magenta color for CRITICAL outputs
        'RESET': '\033[0m'  # Define the default text style
    }

    def __init__(self, level: logging_levels = 'INFO',
                 show_timestamp: bool = True,
                 show_caller_function: bool = True) -> None:
        """
        Initializes the logger instance with specified settings.

        :param level: The desired logging level for the logger.
                      Acceptable values are 'DEBUG', 'INFO', 'WARNING', 'ERROR',
                      'CRITICAL'.
        :type level: logging_levels
        :param show_timestamp: Indicates whether to display the timestamp in log
                               messages.
        :type show_timestamp: bool
        :param show_caller_function: Indicates whether to display the caller
                                     function name in log messages.
        :type show_caller_function: bool

        :return: None
        """
        self.level = ''
        self.setLevel(level)
        self.show_timestamp = show_timestamp
        self.show_caller_function = show_caller_function

    def setLevel(self, level: logging_levels = 'INFO') -> None:
        """
        Sets the logging level for the instance.

        :param level: The desired logging level. Acceptable values are
                      'DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL'.
        :type level: logging_levels
        :return: None
        """
        if level not in self.logging_levels:
            raise ValueError(
                f"Invalid logging level: '{level}'. "
                f"Acceptable values are: {', '.join(self.logging_levels)}."
            )

        self.level = level

    def setFormat(self, show_timestamp: bool = True,
                  show_caller_function: bool = True) -> None:
        """
        Sets the format options for logging messages.

        :param show_timestamp: Indicates whether to display the timestamp in
                               logs.
        :type show_timestamp: bool
        :param show_caller_function: Indicates whether to display the caller
                                     function name in logs.
        :type show_caller_function: bool

        :return: None
        """
        self.show_timestamp = show_timestamp
        self.show_caller_function = show_caller_function

    def _log(self, print_level: logging_levels, *values: object) -> None:
        """
        Logs a message with the specified logging level and caller function
        information.

        :param print_level: The level of logging (e.g., DEBUG, INFO, WARNING).
        :type print_level: logging_levels
        :param caller_function: The name of the function that is logging the
                                message.
        :type caller_function: str
        :param values: The values to be logged.
        :type values: object

        :return: None
        """
        if (self.logging_levels.index(print_level) >=
                self.logging_levels.index(self.level)):
            print(self.logging_colors[print_level], end='')

            # Print the current timestamp if enabled
            if self.show_timestamp:
                # Get the current date and time
                current_time = (datetime.datetime.now().
                                strftime('%Y-%m-%d %H:%M:%S'))
                print(f'[{current_time}] - ', end='')

            # Print the logging level
            print(f'{print_level} - ', end='')

            # Print the caller function if enabled
            if self.show_caller_function:
                # Get the name of the function that called error
                caller_funct = (inspect.currentframe().
                                f_back.f_back.f_code.co_name)
                print(f'From ({caller_funct}): ', end='')

            # Print the actual message values
            print(*values, end='')

            # Reset the color
            print(self.logging_colors['RESET'])

    def debug(self, *values: object) -> None:
        """
        Logs messages at the DEBUG level.

        :param values: Messages to be logged.
        :type values: object

        :return: None
        """
        self._log('DEBUG', *values)

    def info(self, *values: object) -> None:
        """
        Logs messages at the INFO level.

        :param values: Messages to be logged.
        :type values: object

        :return: None
        """
        self._log('INFO', *values)

    def warning(self, *values: object) -> None:
        """
        Logs messages at the WARNING level.

        :param values: Messages to be logged.
        :type values: object

        :return: None
        """
        self._log('WARNING', *values)

    def error(self, *values: object) -> None:
        """
        Logs messages at the ERROR level.

        :param values: Messages to be logged.
        :type values: object

        :return: None
        """
        self._log('ERROR', *values)

    def critical(self, *values: object) -> None:
        """
        Logs messages at the CRITICAL level.

        :param values: Messages to be logged.
        :type values: object

        :return: None
        """
        self._log('CRITICAL', *values)

# end of file #
